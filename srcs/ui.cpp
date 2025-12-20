#include "ui.h"
#include <QtCore/qnamespace.h>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QWidget>

namespace Ui {
    void MainWindow::setupUi(QMainWindow* mainWindow) {
        QWidget* centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        centralWidget->setMinimumSize(800, 600);

        mainWindow->setWindowTitle("访问认证");
        QString iconPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/icon.png");
        if (QFileInfo::exists(iconPath))
            mainWindow->setWindowIcon(QIcon(iconPath));
        else
            QMessageBox::warning(nullptr, "Warning", "Icon file does not exist!");
    }
}; // namespace Ui