#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "encoder.h"

/**
 * @brief 生成二进制配置文件
 * @return 成功 ``true`` | 失败 ``false``
 */
bool generateConfig(const QString& csvPath, const QString& binPath) {
    /** @hiderefs
     * CSV格式: 标识符|明文密码|文件夹路径
     * 注意分隔符是 “|” 而非 “,”，因此在标识符、密码、文件夹路径中均不可出现该符号！
     * BIN格式:
     *   [4字节] 标识符数量
     *   循环每个标识符:
     *     [4字节] 标识符长度 + [N字节] 标识符 (UTF-8)
     *     [4字节] 密码哈希长度 + [N字节] 哈希 (ASCII)
     *     [4字节] 文件夹路径长度 + [N字节] 路径 (UTF-8)
     */
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QFile csvFile(csvPath);
    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Unable to open CSV file: " << csvPath;
        return false;
    }

    struct AccessData {
        QString identifier;
        QString passwordHash;
        QString folderPath;
    };
    QList<AccessData> tags;

    QTextStream stream(&csvFile); // @hiderefs 默认UTF-8
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))  // 支持以行首 # 作为注释
            continue;
        QStringList parts = line.split('|');
        if (parts.size() != 3) {
            cout << "Invalid line skipped: " << line << "\n" << Qt::flush;
            continue;
        }

        AccessData tag;
        tag.identifier = parts[0].trimmed();
        tag.passwordHash = password_encode(parts[1].trimmed());  // 不存储明文
        tag.folderPath = parts[2].trimmed();
        if (!tag.identifier.isEmpty() && !tag.passwordHash.isEmpty())
            tags.append(tag);
    }
    csvFile.close();
    if (tags.isEmpty()) {
        cout << "No valid user configuration found!";
        return false;
    }
    QFile binFile(binPath);
    if (!binFile.open(QIODevice::WriteOnly)) { // 写入二进制
        cout << "Unable to create binary file:" << binPath;
        return false;
    }

    QDataStream out(&binFile);
    out.setVersion(QDataStream::Qt_6_0);
    // 写入标识符数量
    out << static_cast<quint32>(tags.size());
    for (const auto& tag : tags) {
        // 标识符
        QByteArray idUtf8 = tag.identifier.toUtf8();
        out << static_cast<quint32>(idUtf8.length());
        out.writeRawData(idUtf8.constData(), idUtf8.length());
        // 密码哈希
        QByteArray hashAscii = tag.passwordHash.toLatin1();
        out << static_cast<quint32>(hashAscii.length());
        out.writeRawData(hashAscii.constData(), hashAscii.length());
        // 跳转路径
        QByteArray pathUtf8 = tag.folderPath.toUtf8();
        out << static_cast<quint32>(pathUtf8.length());
        out.writeRawData(pathUtf8.constData(), pathUtf8.length());
    }
    binFile.close();
    cout << "Data amount: " << tags.size() << "\n";
    return true;
}

int main(int argc, char* argv[]) {
    system("chcp 65001 > nul");
    QCoreApplication app(argc, argv);
    QTextStream cout(stdout);
    QTextStream cin(stdin);

    cout << "Please enter the path of the CSV file to be read: " << Qt::flush;
    QString csvPath = cin.readLine().trimmed();
    QString binPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/core.bin");
    bool success = generateConfig(csvPath, binPath);
    if (success) {
        cout << "\nOperation completed! Configuration file has been generated to: "
            << binPath << "\n" << Qt::flush;
        system("echo Press any key to exit... & pause > nul");
        return 0;
    }
    else {
        cout << "\n" << Qt::flush;
        system("echo Press any key to exit... & pause > nul");
        return 1;
    }
}