#include <QApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include "encoder.h"
#include "HelpDialog.h"

/**
 * @brief 生成二进制配置文件
 * @return 成功：数量 | 失败：-1
 */
int generateConfig(const QString& txtPath, const QString& binPath) {
    /** @hiderefs
     * TXT格式: 标识符|明文密码|文件夹路径
     * 分隔符是 “|”，因此在标识符、密码、文件夹路径中均不可出现该符号；标识符不得重复
     * BIN格式:
     *   [4字节] 标识符数量
     *   循环每个标识符:
     *     [4字节] 标识符长度 + [N字节] 标识符 (UTF-8)
     *     [4字节] 密码哈希长度 + [N字节] 哈希 (ASCII)
     *     [4字节] 文件夹路径长度 + [N字节] 路径 (UTF-8)
     */
    QFile txtFile(txtPath);
    if (!txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Unable to open TXT file:\n" + txtPath);
        return -1;
    }

    struct AccessData {
        QString identifier;
        QString passwordHash;
        QString folderPath;
    };
    QList<AccessData> tags;
    QSet<QString> dedupIds;
    int lineCnt = 0;

    QTextStream stream(&txtFile); // @hiderefs 默认UTF-8
    bool invalid_line = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        lineCnt++;
        if (line.isEmpty() || line.startsWith('#'))  // 支持以行首 # 作为注释
            continue;
        QStringList parts = line.split('|');
        if (parts.size() != 3) {
            invalid_line = true;
            continue;
        }
        QString currentId = parts[0].trimmed();
        if (dedupIds.contains(currentId)) {
            txtFile.close();
            QMessageBox::critical(nullptr, "Error",
                "Same identifier is contained in the configuration file!\n" +
                QString("配置文件中包含了相同的标识符！\n\nLine: %1    ID: %2").arg(lineCnt).arg(currentId)
            );
            return -1;
        }
        AccessData tag;
        tag.identifier = currentId;
        tag.passwordHash = passwordEncode(parts[1].trimmed());  // 不存储明文
        tag.folderPath = parts[2].trimmed();
        if (!tag.identifier.isEmpty() && !tag.passwordHash.isEmpty()) {
            tags.append(tag);
            dedupIds.insert(currentId);
        }
    }
    txtFile.close();
    if (invalid_line)
        QMessageBox::warning(nullptr, "Warining", "Some invalid lines were skipped!");
    if (tags.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No valid user configuration found!");
        return -1;
    }
    QFile binFile(binPath);
    if (!binFile.open(QIODevice::WriteOnly)) { // 写入二进制
        QMessageBox::critical(nullptr, "Write Error", "Unable to create binary file:\n" + binPath);
        return -1;
    }
    QDataStream out(&binFile);
    out.setVersion(QDataStream::Qt_6_0);
    // 写入标识符数量
    out << static_cast<quint32>(tags.size());
    for (const auto& tag : tags) {
        // 标识符（混淆）
        QByteArray idUtf8 = obfString(tag.identifier).toUtf8();
        out << static_cast<quint32>(idUtf8.length());
        out.writeRawData(idUtf8.constData(), idUtf8.length());
        // 密码（哈希加密）
        QByteArray hashAscii = tag.passwordHash.toLatin1();
        out << static_cast<quint32>(hashAscii.length());
        out.writeRawData(hashAscii.constData(), hashAscii.length());
        // 跳转路径（混淆）
        QByteArray pathUtf8 = obfString(tag.folderPath).toUtf8();
        out << static_cast<quint32>(pathUtf8.length());
        out.writeRawData(pathUtf8.constData(), pathUtf8.length());
    }
    binFile.close();
    return tags.size();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMessageBox* msgbox_how2use = new QMessageBox();
    msgbox_how2use->setWindowTitle("How to use");
    msgbox_how2use->setText(
        "1. Select the TXT file with your configurations to be read.\n"
        "2. The content of the TXT file contains an identifier, a password, and a folder path that jumps after verification, "
        "which are separated by a symbol \"|\" with no spaces on both sides, i.e.:\n"
        "\tIdentifier|Password|Folder Path\n"
        "3. If the software is used for removable storage media (CD-ROM, USB flash drive, removable hard disk, etc.), "
        "please change the drive letter of the \"Jump Folder Path\" to \"Z:\" to avoid the drive letter change of this storage media. "
        "The software will automatically identify the so-called \"Z:\" and redirect to the drive letter where the software is located.\n"
        "4. In the TXT file, the # at the beginning of the line is used as a comment. "
        "Identifiers cannot contain duplicates. Passwords should be composed of letters, numbers and symbols.\n"
        "5. After reading, the number of valid configuration information will be displayed. "
        "The configuration file will be generated automatically, and then the main software can be used normally.\n\n"
        "1. 选择包含待读取配置的 TXT 文件。\n"
        "2. TXT 文件的内容包含标识符、密码、验证后跳转的文件夹路径，两两用符号 “|” 分隔，注意两侧不加空格，即：\n"
        "\t标识符|密码|文件夹路径\n"
        "3. 若软件用于可移动存储介质（光盘、U盘、移动硬盘等），请将 “跳转文件夹路径” 的盘符改为 “Z:”，避免盘符更改。"
        "软件会自动识别所谓的Z盘，并重定向至软件所在盘符。\n"
        "4. TXT 文件中，行首的 # 可用于注释。标识符不能重复，密码应由英文字母、数字、符号构成。\n"
        "5. 文件读取后会显示有效配置信息的数量，配置文件会自动生成，此后即可正常使用主软件。\n\n"
        "Read the instructions carefully, then select the file by clicking OK.\n"
        "请仔细阅读使用须知，然后点击 OK 选择文件。"
    );
    msgbox_how2use->setIcon(QMessageBox::NoIcon);
    msgbox_how2use->setWindowFlags(msgbox_how2use->windowFlags() & ~Qt::WindowCloseButtonHint); // 禁用窗口关闭
    msgbox_how2use->setStandardButtons(QMessageBox::Ok | QMessageBox::Close);
    // QAbstractButton* helpBtn = msgbox_how2use->addButton("Help", QMessageBox::InvalidRole);
    QPushButton* msgOkBtn = qobject_cast<QPushButton*>(msgbox_how2use->button(QMessageBox::Ok));
    if (msgOkBtn)
        msgOkBtn->setEnabled(false);
    QTimer::singleShot(3000, [msgbox_how2use]() {
        QPushButton* btn = qobject_cast<QPushButton*>(msgbox_how2use->button(QMessageBox::Ok));
        if (btn)
            btn->setEnabled(true);
        });
    // QObject::connect(helpBtn, &QPushButton::clicked, msgbox_how2use, []() {
    //     HelpDialog* dlg = new HelpDialog();
    //     dlg->setAttribute(Qt::WA_DeleteOnClose);
    //     dlg->show();  // 非模态显示
    //     });
    auto ret = msgbox_how2use->exec();
    msgbox_how2use->deleteLater();
    if (ret == QMessageBox::Close)
        return 0;
    QString txtPath = QFileDialog::getOpenFileName(
        nullptr,
        "Select configuration TXT file",
        QDir::currentPath(),
        "Text Files (*.txt);;All Files (*)"
    );
    if (txtPath.isEmpty()) {
        QMessageBox::information(nullptr, "Cancelled", "No file selected!");
        return 0;
    }
    QString binPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/core.bin");
    int n_tags = generateConfig(txtPath, binPath);
    if (n_tags < 0)
        return 1;
    else {
        QMessageBox::information(nullptr, "Success",
            "Data amount: " + QString::number(n_tags) + "\nConfiguration file has been generated."
        );
        return 0;
    }
}