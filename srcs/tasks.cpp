#include "tasks.h"
#include <QDataStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QMessageBox>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

// 当前 chunk 大小: 8 MB
static const qint64 IO_CHUNK_SIZE = 8 * 1024 * 1024;
static const char MAGIC[4] = {'F', 'D', 'B', 'P'};  // FDBP: folder-database package
static const quint16 VERSION = 1;

// ==================== Protect Task ====================

ProtectTask::ProtectTask(QString folder, QString db)
    : folderPath(std::move(folder)), dbPath(std::move(db)) {
    setAutoDelete(true);
    m_cancel.store(false);
}

void ProtectTask::run() {
    if (isCanceled()) {
        emit finished(false, QStringLiteral("Operation Canceled!"));
        return;
    }
    int ok = folder2db(folderPath, dbPath);
    if (ok) {  // TODO: 细化错误分支
        emit finished(false, "Failed to protect folder!\n保护文件夹失败！");
        return;
    }
    // ok == 12:
    // QMessageBox::warning(nullptr, "Warning",
    //         "Failed to delete individual files due to occupation! "
    //         "To ensure data security, please delete the original folder manually.\n"
    //         "个别文件因占用而删除失败！为确保数据安全，请手动删除原文件夹。\n"
    //         "Otherwise, the next time you visit the folder, "
    //         "you won't be shown all the files until you manually delete the incomplete folder.\n"
    //         "否则，下次访问该文件夹时，将不会为您展示全部文件，直到您手动删除。\n\n"
    //         "Folder Path: " + folderPath
    //     );
    protectDbFile(dbPath);
    emit finished(true, "Folder has been protected successfully.\n文件夹已成功保护。");
}

void ProtectTask::requestCancel() {
    m_cancel.store(true);
}

bool ProtectTask::isCanceled() const { return m_cancel.load(); }

// ==================== Restore Task ====================

RestoreTask::RestoreTask(QString db, QString folder)
    : dbPath(std::move(db)), folderPath(std::move(folder)) {
    setAutoDelete(true);
    m_cancel.store(false);
}

void RestoreTask::run() {
    if (isCanceled()) {
        emit finished(false, QStringLiteral("Operation Canceled!"));
        return;
    }
    int ok = db2folder(dbPath, folderPath);
    if (ok) {  // TODO: 细化错误分支
        emit finished(false, "Failed to restore folder!\n还原文件夹失败！");
        return;
    }
    emit finished(true, folderPath);
}

void RestoreTask::requestCancel() {
    m_cancel.store(true);
}

bool RestoreTask::isCanceled() const {
    return m_cancel.load();
}

// ==================== Concrete Realization ====================

/** @brief Operation: pack folder into db file
 *  @return ``0``: success | ``1``: manually cancel |
 *  ``11``: files cannot be read | ``12``: some files cannot be deleted
 */
int ProtectTask::folder2db(const QString& folderPath, const QString& dbPath) {
    quint32 totalFiles = 0;  // 文件数量统计
    QDirIterator it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        ++totalFiles;
        if (isCanceled())
            return 1;
    }
    QFile outFile(dbPath);
    if (!outFile.open(QIODevice::WriteOnly))
        return 11;
    QDataStream out(&outFile);
    out.setByteOrder(QDataStream::LittleEndian);

    // ===== 文件头 =====
    out.writeRawData(MAGIC, 4); // 写入 MAGIC
    out << quint16(VERSION);    // 写入 VERSION
    out << totalFiles;  // 写入文件数量
    QDir baseDir(folderPath);

    // ===== 文件内容 =====
    quint32 processedFiles = 0;  // 已处理的文件数量
    QDirIterator w_it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    while (w_it.hasNext()) {
        if (isCanceled()) {
            outFile.close();
            outFile.remove();  // 取消后，删除不完整的 db 文件以避免残留
            return 1;
        }
        QString filePath = w_it.next();
        QFile inFile(filePath);
        if (!inFile.open(QIODevice::ReadOnly)) {
            // 跳过无法打开的文件，但仍计入 processed（或按需不计）
            ++processedFiles;
            emit progress(processedFiles, totalFiles);
            continue;
        }
        QByteArray pathBytes = (baseDir.relativeFilePath(filePath)).toUtf8();
        out << quint16(pathBytes.size());
        out.writeRawData(pathBytes.constData(), pathBytes.size());
        // 写入数据长度
        quint64 size = static_cast<quint64>(inFile.size());
        out << size;
        // 分块读取并写入到 out
        while (!inFile.atEnd()) {
            if (isCanceled()) {
                inFile.close();
                outFile.close();
                outFile.remove();
                return 1;
            }
            QByteArray chunk = inFile.read(IO_CHUNK_SIZE);
            if (chunk.isEmpty()) break;
            out.writeRawData(chunk.constData(), chunk.size());
        }
        inFile.close();
        ++processedFiles;
        emit progress(processedFiles, totalFiles);
    }
    outFile.close();
    QDir dir(folderPath);
    if (!dir.removeRecursively())  // 删除原文件夹
        return 12;
    return 0;
}

/// @brief 对 windows 系统附加系统文件和隐藏属性
bool ProtectTask::protectDbFile(const QString& filePath) {
#ifdef Q_OS_WIN
    std::wstring wpath = filePath.toStdWString();
    DWORD attrs = GetFileAttributesW(wpath.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return false;
    attrs |= FILE_ATTRIBUTE_HIDDEN;
    attrs |= FILE_ATTRIBUTE_SYSTEM;
    return SetFileAttributesW(wpath.c_str(), attrs);
#else
    Q_UNUSED(filePath);
    return false;
#endif
}

/** @brief Operation: restore from db file to folder
 *  @return ``0``: success | ``1``: manually cancel |
 *  ``11``: files cannot be read | ``12``: some files cannot be deleted |
 *  `13`: folder cannot be opened | ``14``: length of the read data illegal |
 *  ``51``: MAGIC verification error | ``52``: VERSION undefined |
 *  ``53``: path length mismatching
 */
int RestoreTask::db2folder(const QString& dbPath, const QString& outputDir) {
    QFile inFile(dbPath);
    if (!inFile.open(QIODevice::ReadOnly))
        return 11;
    QDataStream in(&inFile);
    in.setByteOrder(QDataStream::LittleEndian);
    char magic[4];
    in.readRawData(magic, 4);
    if (memcmp(magic, MAGIC, 4) != 0)
        return 51;  // MAGIC 校验不匹配

    quint16 version;  // 当前代码暂未引入版本校验
    quint32 fileCount;
    in >> version >> fileCount;
    quint32 processed = 0;  // 已处理的文件数量
    for (quint32 i = 0; i < fileCount; ++i) {
        quint16 pathLen;
        in >> pathLen;
        if (isCanceled()) {
            inFile.close();  // 取消则仅退出，不删掉文件
            return 1;
        }
        QByteArray pathBytes(pathLen, 0);
        if (in.readRawData(pathBytes.data(), pathLen) != pathLen) {
            inFile.close();
            return 53;
        }
        QString relativePath = QString::fromUtf8(pathBytes);
        quint64 dataLen;
        in >> dataLen;
        QString outPath = outputDir + "/" + relativePath;
        if (!QDir().mkpath(QFileInfo(outPath).absolutePath()))  // 文件夹路径创建失败
            return 13;
        QFile outFile(outPath);
        if (!outFile.open(QIODevice::WriteOnly)) {  // 无法定位到输出位置
            inFile.close();
            return 13;
        }
        quint64 remain = dataLen;
        const qint64 bufSize = IO_CHUNK_SIZE;
        QByteArray buffer;
        buffer.resize(static_cast<int>(std::min<quint64>(remain, static_cast<quint64>(bufSize))));
        // ===== 分块读取 dataLen 字节并写入 =====
        while (remain > 0) {
            if (isCanceled()) {
                outFile.close();
                inFile.close();
                return 1;
            }
            qint64 toRead = static_cast<qint64>(std::min<quint64>(remain, static_cast<quint64>(bufSize)));
            // 复用 buffer 大小
            buffer.resize(static_cast<int>(toRead));
            qint64 got = in.readRawData(buffer.data(), static_cast<int>(toRead));
            if (got <= 0) {
                outFile.close();
                inFile.close();
                return 14;
            }
            outFile.write(buffer.constData(), static_cast<qint64>(got));
            remain -= static_cast<quint64>(got);
        }
        outFile.close();
        ++processed;
        emit progress(processed, fileCount);
    }
    inFile.close();
    if (!QFile::remove(dbPath))
        return 12;
    return 0;
}