#include "ui.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QLayout>
#include <QMessageBox>
#include <QWidget>

namespace Ui {
    void MainWindow::setupUi(QMainWindow* mainWindow) {
        QWidget* centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        centralWidget->setMinimumSize(300, 400);

        mainWindow->setWindowTitle("Access Authentication");
        QString iconPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/icon.png");
        if (QFileInfo::exists(iconPath))
            mainWindow->setWindowIcon(QIcon(iconPath));
        else
            QMessageBox::warning(nullptr, "Warning", "Icon file does not exist!");

        centralWidget->setStyleSheet(
            "QLabel, QLineEdit, QPushButton { font-size: 14px; }"
        );

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        uiTitle = new QLabel("受保护文件访问认证");
        uiTitle->setStyleSheet("font-size: 16px;");
        uiTitle->setAlignment(Qt::AlignCenter);

        QHBoxLayout* idLayout = new QHBoxLayout(); // identifier layout
        id_label = new QLabel("标识符");
        id_input = new QLineEdit();
        idLayout->addWidget(id_label, 1);
        idLayout->addWidget(id_input, 3);
        QHBoxLayout* pwLayout = new QHBoxLayout(); // password layout
        pw_label = new QLabel("密码");
        pw_input = new QLineEdit();
        pw_input->setEchoMode(QLineEdit::Password);
        pwLayout->addWidget(pw_label, 1);
        pwLayout->addWidget(pw_input, 3);

        go_btn = new QPushButton("确认");
        go_btn->setCursor(Qt::PointingHandCursor);
        go_btn->setFixedWidth(150);

        lang_btn = new QPushButton("EN / 英");
        lang_btn->setCursor(Qt::PointingHandCursor);
        lang_btn->setStyleSheet(R"(
    QPushButton {
        border: none;
        padding: 8px 20px;
        border-radius: 4px;
        background: #E0E0E0;
        font-size: 14px;
    }
    QPushButton:checked {
        background: #2196F3;
        color: white;
    }
        )");
        lang_btn->setFixedWidth(150);

        mainLayout->addWidget(uiTitle, 10);
        mainLayout->addLayout(idLayout, 10);
        mainLayout->addLayout(pwLayout, 10);
        mainLayout->addWidget(go_btn, 10);
        mainLayout->addWidget(lang_btn, 10);
    }
}; // namespace Ui