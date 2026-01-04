#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <vector>

/// @brief 加载风格表
/// @param app
/// @param files 由各 QFile 文件的引用组成，个数无限制，一般指向 QSS 文件
void loadStyleSheet(QApplication &app, const std::vector<QFile*> &files) {
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile qssFile1 = QFile(QDir(QCoreApplication::applicationDirPath()).filePath("resources/button.qss")),
    qssFile2 = QFile(QDir(QCoreApplication::applicationDirPath()).filePath("resources/ui_bg.qss"));
    std::vector<QFile*> qss_files = {&qssFile1, &qssFile2};
    loadStyleSheet(app, qss_files);

    MainWindow win;
    win.show();
    return app.exec();
}
