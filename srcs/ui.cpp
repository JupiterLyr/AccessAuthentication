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

        lang_btn = new SlideButton("EN / 英");
        lang_btn->setCursor(Qt::PointingHandCursor);
        // lang_btn->setCheckable(true);
        // lang_btn->setStyleSheet(R"(
        //     QPushButton {
        //         border: none;
        //         padding: 8px 20px;
        //         border-radius: 4px;
        //         font-size: 14px;
        //         min-width: 150px;
                
        //         /* 初始状态：左深色(#2196F3) 右浅色(#D8F1F5) */
        //         background: qlineargradient(
        //             x1:0, y1:0, x2:1, y2:0,
        //             stop:0 #2196F3, stop:0.49 #2196F3,  /* 左半部分 */
        //             stop:0.5 #D8F1F5, stop:1 #D8F1F5    /* 右半部分 */
        //         );
        //         color: #333;
        //     }
        //     QPushButton:hover {
        //         background: qlineargradient(
        //             x1:0, y1:0, x2:1, y2:0,
        //             stop:0 #1976D2, stop:0.49 #1976D2,
        //             stop:0.5 #C8E6F5, stop:1 #C8E6F5
        //         );
        //     }
        //     QPushButton:checked {
        //         /* 选中状态：左浅色 右深色 */
        //         background: qlineargradient(
        //             x1:0, y1:0, x2:1, y2:0,
        //             stop:0 #D8F1F5, stop:0.49 #D8F1F5,
        //             stop:0.5 #2196F3, stop:1 #2196F3
        //         );
        //         color: white;
        //     }
        //     QPushButton:checked:hover {
        //         background: qlineargradient(
        //             x1:0, y1:0, x2:1, y2:0,
        //             stop:0 #C8E6F5, stop:0.49 #C8E6F5,
        //             stop:0.5 #1976D2, stop:1 #1976D2
        //         );
        //     }
        //     )"
        // );
        lang_btn->setFixedWidth(150);

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