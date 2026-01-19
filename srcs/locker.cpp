#include <QDataStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>

bool folder2db(const QString& folderPath, const QString& dbPath) {
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
    return true;
}

bool db2folder(const QString& dbPath, const QString& outputDir) {
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
    return true;
}