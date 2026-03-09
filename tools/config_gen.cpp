#include <QApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QLocalSocket>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include "encoder.h"
#include "HelpDialog.h"

class How2Use : public QDialog {
public:
    explicit How2Use(QWidget* parent = nullptr);

private:
    QPushButton* okBtn;
    QPushButton* helpBtn;
    QPushButton* cancelBtn;
};

How2Use::How2Use(QWidget* parent) : QDialog(parent) {
    setWindowTitle("How to use");
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    QLabel* text = new QLabel(
        "If you don\'t know how to use it, please click \"Help\" to view \"How to Customize The Registration\".\n"
        "Click \"OK\" to select the TXT file whose configuration you have written, or click \"Close\" to exit.\n"
        "若不清楚如何使用，请点击 Help 查看“定制化注册配置的方法”。\n"
        "点击 OK 选择已配置好的 TXT 文件，点击 Close 退出。"
    );
    text->setWordWrap(true);
    okBtn = new QPushButton("OK");
    helpBtn = new QPushButton("Help");
    cancelBtn = new QPushButton("Close");
    okBtn->setEnabled(false);
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(helpBtn);
    btnLayout->addWidget(cancelBtn);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(text);
    mainLayout->addLayout(btnLayout);

    connect(helpBtn, &QPushButton::clicked, this, [this] {
        static HelpDialog* dlg = nullptr;
        if (!dlg) {
            dlg = new HelpDialog(this);
            dlg->setAttribute(Qt::WA_DeleteOnClose);
            connect(dlg, &QObject::destroyed, this, [] { dlg = nullptr; });
            dlg->show();
        }
        else {
            dlg->raise();
            dlg->activateWindow();
        }}
    );
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    QTimer::singleShot(3000, this, [this] { okBtn->setEnabled(true); });
}

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

bool isMainAppRunning() {
    QLocalSocket socket;
    socket.connectToServer("AA_Core_Server");
    return socket.waitForConnected(200);
}

void requestMainAppClose() {
    QLocalSocket socket;
    socket.connectToServer("AA_Core_Server");
    if (!socket.waitForConnected(300))
        return;
    socket.write("EXIT");
    socket.flush();
    socket.waitForBytesWritten(300);
    socket.disconnectFromServer();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    if (isMainAppRunning()) {
        auto ret = QMessageBox::question(nullptr, "Main App Is Running",
            "AccessAuthentication.exe is currently running!\n"
            "Configuration changes will not take effect until it is restarted.\n\n"
            "Do you want to close AccessAuthentication.exe now?",
            QMessageBox::Yes | QMessageBox::No
        );
        if (ret == QMessageBox::Yes)
            requestMainAppClose();
        else {
            QMessageBox::warning(nullptr, "Warning",
                "The new configuration will take effect only after restarting AccessAuthentication.exe!\n"
                "新配置仅会在重启主软件 AccessAuthentication.exe 后会生效！"
            );
        }
    }
    How2Use dlg;
    if (dlg.exec() != QDialog::Accepted)
        return 0;
    QString txtPath = QFileDialog::getOpenFileName(
        nullptr,
        "Select configuration TXT file",
        QDir::currentPath(),
        "Text Files (*.txt);;All Files (*)"
    );
    if (txtPath.isEmpty()) {
        QMessageBox::information(nullptr, "Cancelled", "No file selected!");
        return 1;
    }
    QString binPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/core.bin");
    int n_tags = generateConfig(txtPath, binPath);
    if (n_tags < 0)
        return 2;
    else {
        QMessageBox::information(nullptr, "Success",
            "Data amount: " + QString::number(n_tags) + "\nConfiguration file has been generated."
        );
        return 0;
    }
}