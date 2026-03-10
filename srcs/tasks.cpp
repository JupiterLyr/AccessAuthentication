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
static const char MAGIC[4] = { 'F', 'D', 'B', 'P' };  // FDBP: folder-database package
static const quint16 VERSION = 2;

enum TaskResult {
    TASK_OK = 0,
    TASK_CANCEL = 1,

    // ===== ERROR =====
    TASK_FILE_OPEN_ERROR = 11,      // cannot open the file
    TASK_FILE_READ_FAILED = 12,     // some files cannot be read
    TASK_FILE_DELETE_FAILED = 13,   // some files cannot be deleted
    TASK_FOLDER_CREATE_FAILED = 14, // cannot create the folder

    TASK_DATA_ILLEGAL = 21,         // data in the file is illegal

    TASK_VERIFY_MAGIC = 51,         // magic text verification error
    TASK_VERIFY_VERSION = 52,       // inconsistent versions
    TASK_VERIFY_PATH_LEN = 53,      // path length mismatching in database

    // ===== WARNING =====
    TASK_WARN_DB_REMAIN = 71        // database file is not deleted properly
};

// ==================== Protect Task ====================

ProtectTask::ProtectTask(QString folder, QString db)
    : folderPath(std::move(folder)), dbPath(std::move(db)) {
}

void ProtectTask::run() {
    if (isCanceled()) {
        emit finished(1, QStringLiteral("Operation Canceled!"));
        return;
    }
    int ret = folder2db(folderPath, dbPath);
    if (ret) {  // TODO: 细化错误分支
        emit finished(ret, "Failed to protect folder!\n保护文件夹失败！");
        return;
    }
    // ok == 13:
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
    emit finished(0, "Folder has been protected successfully.\n文件夹已成功保护。");
}

// ==================== Restore Task ====================

RestoreTask::RestoreTask(QString db, QString folder)
    : dbPath(std::move(db)), folderPath(std::move(folder)) {
}

void RestoreTask::run() {
    if (isCanceled()) {
        emit finished(1, QStringLiteral("Operation Canceled!"));
        return;
    }
    int ret = db2folder(dbPath, folderPath);
    if (ret) {  // TODO: 细化错误分支
        emit finished(ret, "Failed to restore folder!\n还原文件夹失败！");
        return;
    }
    emit finished(0, folderPath);
}

// ==================== Realization ====================

int ProtectTask::folder2db(const QString& folderPath, const QString& dbPath) {
    quint64 totalBytes = 0;  // 文件字节数统计
    QDirIterator it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo info(it.next());  // 获取文件信息
        totalBytes += static_cast<quint64>(info.size());
        if (isCanceled())
            return TASK_CANCEL;
    }
    QFile outFile(dbPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        outFile.close();
        outFile.remove();
        return TASK_FILE_OPEN_ERROR;
    }
    QDataStream out(&outFile);
    out.setByteOrder(QDataStream::LittleEndian);

    // ===== 文件头 =====
    out.writeRawData(MAGIC, 4); // 写入 MAGIC
    out << quint16(VERSION);    // 写入 VERSION
    out << totalBytes;  // 写入所有文件总大小
    QDir baseDir(folderPath);

    // ===== 文件内容 =====
    quint64 processedBytes = 0;  // 已处理的文件数量
    quint16 chunkCounter = 0;  // 用于记录块的个数，控制更新进度条的时机
    QByteArray buffer;
    buffer.resize(IO_CHUNK_SIZE);
    QDirIterator w_it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    while (w_it.hasNext()) {
        if (isCanceled()) {
            outFile.close();
            outFile.remove();  // 取消后，删除不完整的 db 文件以避免残留
            return TASK_CANCEL;
        }
        QString filePath = w_it.next();
        QFile inFile(filePath);
        if (!inFile.open(QIODevice::ReadOnly))  // 存在无法打开的文件
            return TASK_FILE_READ_FAILED;
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
                return TASK_CANCEL;
            }
            qint64 n = inFile.read(buffer.data(), IO_CHUNK_SIZE);
            if (n <= 0) break;
            out.writeRawData(buffer.constData(), n);
            processedBytes += static_cast<quint64>(n);
            chunkCounter++;
            if (chunkCounter % 10 == 0)
                emit progress(processedBytes, totalBytes);
        }
        inFile.close();
    }
    emit progress(processedBytes, totalBytes);
    outFile.close();
    QDir dir(folderPath);
    if (!dir.removeRecursively())  // 删除原文件夹
        return TASK_FILE_DELETE_FAILED;
    return TASK_OK;
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

int RestoreTask::db2folder(const QString& dbPath, const QString& outputDir) {
    QFile inFile(dbPath);
    if (!inFile.open(QIODevice::ReadOnly))
        return TASK_FILE_OPEN_ERROR;
    QDataStream in(&inFile);
    in.setByteOrder(QDataStream::LittleEndian);
    char magic[4];
    in.readRawData(magic, 4);
    if (memcmp(magic, MAGIC, 4) != 0)  // MAGIC 校验不匹配
        return TASK_VERIFY_MAGIC;

    quint16 version;
    quint64 totalBytes;
    in >> version >> totalBytes;
    if (version != VERSION)  // VERSION 不适配当前代码
        return TASK_VERIFY_VERSION;
    quint64 processedBytes = 0;  // 已处理的文件大小
    quint16 chunkCounter = 0;  // 用于记录块的个数，控制更新进度条的时机
    QByteArray buffer;
    buffer.resize(IO_CHUNK_SIZE);
    while (!in.atEnd()) {
        if (isCanceled()) {
            inFile.close();  // 取消则仅退出，不删掉文件
            return TASK_CANCEL;
        }
        quint16 pathLen;
        in >> pathLen;
        QByteArray pathBytes(pathLen, 0);
        if (in.readRawData(pathBytes.data(), pathLen) != pathLen) {
            inFile.close();
            return TASK_VERIFY_PATH_LEN;
        }
        QString relativePath = QString::fromUtf8(pathBytes);
        quint64 dataLen;
        in >> dataLen;
        // QString outPath = outputDir + "/" + relativePath;
        QString outPath = QDir(outputDir).filePath(relativePath);
        if (!QDir().mkpath(QFileInfo(outPath).absolutePath())) {  // 文件夹路径创建失败
            inFile.close();
            return TASK_FOLDER_CREATE_FAILED;
        }
        QFile outFile(outPath);
        if (!outFile.open(QIODevice::WriteOnly)) {  // 无法定位到输出位置
            inFile.close();
            return TASK_FILE_OPEN_ERROR;
        }
        quint64 remain = dataLen;

        // ===== 分块读取 dataLen 字节并写入 =====
        while (remain > 0) {
            if (isCanceled()) {
                outFile.close();
                inFile.close();
                return TASK_CANCEL;
            }
            qint64 toRead = std::min<quint64>(remain, IO_CHUNK_SIZE);
            qint64 got = in.readRawData(buffer.data(), static_cast<int>(toRead));
            if (got <= 0) {
                outFile.close();
                QFile::remove(outPath);
                inFile.close();
                return TASK_DATA_ILLEGAL;
            }
            outFile.write(buffer.constData(), got);
            remain -= static_cast<quint64>(got);
            processedBytes += static_cast<quint64>(got);
            chunkCounter++;
            if (chunkCounter % 10 == 0)
                emit progress(processedBytes, totalBytes);
        }
        outFile.close();
    }
    emit progress(processedBytes, totalBytes);
    inFile.close();
    normalizeDbFile(dbPath);
    if (!QFile::remove(dbPath))
        return TASK_WARN_DB_REMAIN;
    return TASK_OK;
}

/// @brief 取消 windows 系统文件和隐藏属性
bool RestoreTask::normalizeDbFile(const QString& filePath) {
#ifdef Q_OS_WIN
    std::wstring wpath = filePath.toStdWString();
    DWORD attrs = GetFileAttributesW(wpath.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return false;
    attrs &= ~FILE_ATTRIBUTE_HIDDEN;
    attrs &= ~FILE_ATTRIBUTE_SYSTEM;
    return SetFileAttributesW(wpath.c_str(), attrs);
#else
    Q_UNUSED(filePath);
    return false;
#endif
}