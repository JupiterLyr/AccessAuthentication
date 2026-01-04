#include "ui.h"
#include <QCoreApplication>
#include <QColor>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QLayout>
#include <QMessageBox>
#include <QWidget>
#include "SlideButton.h"

namespace Ui {
    void MainWindow::setupUi(QMainWindow* mainWindow) {
        QWidget* centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        centralWidget->setMinimumSize(320, 280);

        mainWindow->setWindowTitle("Access Authentication");
        QString iconPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/icon.png");
        if (QFileInfo::exists(iconPath))
            mainWindow->setWindowIcon(QIcon(iconPath));
        else
            QMessageBox::warning(nullptr, "Warning", "Icon file does not exist!");

        centralWidget->setStyleSheet(
            "QLabel, QLineEdit, QPushButton { font-size: 14px; font: Arial; }"
        );

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(20, 20, 20, 20); // Left, Up, Right, Bottom

        uiTitle = new QLabel("受保护文件访问认证");
        uiTitle->setStyleSheet("font-size: 16px;");
        uiTitle->setAlignment(Qt::AlignCenter);
        uiTitle->setFixedHeight(50);

        QHBoxLayout* idLayout = new QHBoxLayout(); // identifier layout
        id_label = new QLabel("标识符");
        id_label->setAlignment(Qt::AlignCenter);
        id_input = new QLineEdit();
        idLayout->addWidget(id_label, 1);
        idLayout->addWidget(id_input, 3);
        QHBoxLayout* pwLayout = new QHBoxLayout(); // password layout
        pw_label = new QLabel("密码");
        pw_label->setAlignment(Qt::AlignCenter);
        pw_input = new QLineEdit();
        pw_input->setEchoMode(QLineEdit::Password);
        pwLayout->addWidget(pw_label, 1);
        pwLayout->addWidget(pw_input, 3);

        go_btn = new QPushButton("确认");
        go_btn->setCursor(Qt::PointingHandCursor);
        go_btn->setFixedWidth(150);

        lang_btn = new SlideButton("　　English");
        lang_btn->setCursor(Qt::PointingHandCursor);
        lang_btn->setFixedWidth(120);
        lang_btn->setSwitchColors({
            QColor(113, 205, 245), // rgb(113, 205, 245)
            QColor(71, 218, 88), // rgb(71, 218, 88)
            QColor(255, 255, 255), // rgb(255, 255, 255)
            QColor(255, 255, 255) // rgb(255, 255, 255)
            });
        lang_btn->setThumbRadius(14);
        lang_btn->setTrackHeight(24);

        idLayout->setAlignment(Qt::AlignCenter);
        pwLayout->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(uiTitle);
        mainLayout->addLayout(idLayout);
        mainLayout->addLayout(pwLayout);
        mainLayout->addWidget(go_btn);
        mainLayout->setAlignment(go_btn, Qt::AlignCenter);
        mainLayout->addWidget(lang_btn);
        mainLayout->setAlignment(lang_btn, Qt::AlignCenter);
    }
}; // namespace Ui