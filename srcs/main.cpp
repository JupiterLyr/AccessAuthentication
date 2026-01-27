#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <vector>

/// @brief 加载风格表
/// @param app
/// @param files 由各 QFile 文件的引用组成，个数无限制，一般指向 QSS 文件
void loadStyleSheet(QApplication& app, const std::vector<QFile*>& files) {
    for (QFile* file : files) {
        if (file->open(QFile::ReadOnly)) {
            QString existingStyleSheet = app.styleSheet();
            QString newStyleSheet = file->readAll();
            app.setStyleSheet(existingStyleSheet + "\n" + newStyleSheet);
            file->close();
        }
        else {
            qDebug() << "Failed to open stylesheet:" << file->fileName();
        }
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QLocalServer mainServer;
    const QString mainServerName = "AA_Core_Server";
    if (!mainServer.listen(mainServerName)) {
        QLocalServer::removeServer(mainServerName);
        if (!mainServer.listen(mainServerName))
            return 0;
    }
    // 由 IPC 通知退出
    QObject::connect(&mainServer, &QLocalServer::newConnection, [&]() {
        QLocalSocket* client = mainServer.nextPendingConnection();
        client->setParent(&mainServer);
        QObject::connect(client, &QLocalSocket::readyRead, [client]() {
            const QByteArray cmd = client->readAll().trimmed();
            if (cmd == "EXIT")
                QCoreApplication::quit();
            });
        QObject::connect(client, &QLocalSocket::disconnected, client, &QLocalSocket::deleteLater);
        });

    QFile qssFile1 = QFile(QDir(QCoreApplication::applicationDirPath()).filePath("resources/button.qss")),
        qssFile2 = QFile(QDir(QCoreApplication::applicationDirPath()).filePath("resources/ui_bg.qss")),
        qssFile3 = QFile(QDir(QCoreApplication::applicationDirPath()).filePath("resources/line_edit.qss"));
    std::vector<QFile*> qss_files = { &qssFile1, &qssFile2, &qssFile3 };
    loadStyleSheet(app, qss_files);

    MainWindow win;
    win.show();
    return app.exec();
}
