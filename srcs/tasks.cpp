#include "tasks.h"
#include <QDataStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QMessageBox>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

ProtectTask::ProtectTask(QString folder, QString db) {
    folderPath = folder;
    dbPath = db;
    setAutoDelete(true);
}

void ProtectTask::run() {
    bool ok = folder2db(folderPath, dbPath);
    if (!ok) {
        emit finished(false, "Failed to protect folder!\n保护文件夹失败！");
        return;
    }
    protectDbFile(dbPath);
    emit finished(true, "Folder has been protected successfully.\n文件夹已成功保护。");
}

RestoreTask::RestoreTask(QString db, QString folder) {
    dbPath = db;
    folderPath = folder;
    setAutoDelete(true);
}

void RestoreTask::run() {
    bool ok = db2folder(dbPath, folderPath);
    if (!ok) {
        emit finished(false, "Failed to restore folder!\n还原文件夹失败！");
        return;
    }
    emit finished(true, folderPath);
}

// ==================== 具体任务实现 ====================

bool ProtectTask::folder2db(const QString& folderPath, const QString& dbPath) {
    QFile outFile(dbPath);
    if (!outFile.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&outFile);
    out.setByteOrder(QDataStream::LittleEndian);

    // ===== 文件头 =====
    out.writeRawData("FDBG", 4);   // MAGIC
    out << quint16(1);             // VERSION
    QList<QString> files; // 收集文件列表
    QDirIterator it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
        files << it.next();
    out << quint32(files.size());  // FILE_COUNT
    QDir baseDir(folderPath);
    // ===== 文件内容 =====
    for (const QString& filePath : files) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            continue;
        QByteArray data = file.readAll();
        QString relativePath = baseDir.relativeFilePath(filePath);
        QByteArray pathBytes = relativePath.toUtf8();
        out << quint16(pathBytes.size());
        out.writeRawData(pathBytes.data(), pathBytes.size());
        out << quint64(data.size());
        out.writeRawData(data.data(), data.size());
        file.close();
    }
    outFile.close();
    QDir dir(folderPath);
    if (!dir.removeRecursively()) {  // 删除原文件夹
        QMessageBox::warning(nullptr, "Warning",
            "Failed to delete individual files due to occupation! "
            "To ensure data security, please delete the original folder manually.\n"
            "个别文件因占用而删除失败！为确保数据安全，请手动删除原文件夹。\n"
            "Otherwise, the next time you visit the folder, "
            "you won't be shown all the files until you manually delete the incomplete folder.\n"
            "否则，下次访问该文件夹时，将不会为您展示全部文件，直到您手动删除。\n\n"
            "Folder Path: " + folderPath
        );
        return false;
    }
    return true;
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

bool RestoreTask::db2folder(const QString& dbPath, const QString& outputDir) {
    QFile inFile(dbPath);
    if (!inFile.open(QIODevice::ReadOnly))
        return false;
    QDataStream in(&inFile);
    in.setByteOrder(QDataStream::LittleEndian);
    char magic[4];
    in.readRawData(magic, 4);
    if (memcmp(magic, "FDBG", 4) != 0)
        return false;

    quint16 version;
    quint32 fileCount;
    in >> version >> fileCount;
    for (quint32 i = 0; i < fileCount; ++i) {
        quint16 pathLen;
        in >> pathLen;
        QByteArray pathBytes(pathLen, 0);
        in.readRawData(pathBytes.data(), pathLen);
        QString relativePath = QString::fromUtf8(pathBytes);
        quint64 dataLen;
        in >> dataLen;
        QByteArray data(dataLen, 0);
        in.readRawData(data.data(), dataLen);
        QString outPath = outputDir + "/" + relativePath;
        QDir().mkpath(QFileInfo(outPath).absolutePath());
        QFile outFile(outPath);
        outFile.open(QIODevice::WriteOnly);
        outFile.write(data);
        outFile.close();
    }
    inFile.close();
    QFile::remove(dbPath);
    return true;
}