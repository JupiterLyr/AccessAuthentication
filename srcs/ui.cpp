#include "ui.h"
#include <QCoreApplication>
#include <QColor>
#include <QDir>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QLayout>
#include <QMessageBox>
#include <QWidget>
#include "DragWidget.h"
#include "SlideButton.h"

namespace Ui {
    void MainWindow::setupUi(QMainWindow* mainWindow) {
        int MAIN_WIDTH = 320, MAIN_HEIGHT = 280, BLUR_RADIUS = 16;
        QWidget* centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        centralWidget->setFixedSize(MAIN_WIDTH + int(BLUR_RADIUS * 1.5), MAIN_HEIGHT + int(BLUR_RADIUS * 1.5));
        centralWidget->setObjectName("centralWidget");

        mainWindow->setWindowTitle("Access Authentication");
        QString iconPath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/icon.png");
        if (QFileInfo::exists(iconPath))
            mainWindow->setWindowIcon(QIcon(iconPath));
        else
            QMessageBox::warning(nullptr, "Warning", "Icon file does not exist!");

        centralWidget->setStyleSheet(
            "QLabel, QLineEdit, QPushButton { font-size: 14px; font: Arial; }"
        );

        QGridLayout* centralLayout = new QGridLayout(centralWidget); // 锚定中央区域
        centralLayout->setSpacing(0);
        QWidget* mainWidget = new QWidget(centralWidget); // 主要部件区
        mainWidget->setObjectName("mainWidget");
        mainWidget->setFixedSize(MAIN_WIDTH, MAIN_HEIGHT);
        mainWidget->setAttribute(Qt::WA_StyledBackground, true);
        centralLayout->addWidget(mainWidget, 0, 0, Qt::AlignCenter);

        QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
        mainLayout->setContentsMargins(24, 18, 26, 18); // Left, Up, Right, Bottom

        uiTitle = new QLabel("受保护文件访问认证");
        uiTitle->setStyleSheet("font-size: 13pt; font-weight: bold;");
        uiTitle->setAlignment(Qt::AlignCenter);
        uiTitle->setFixedHeight(50);

        QHBoxLayout* idLayout = new QHBoxLayout(mainWidget); // identifier layout
        id_label = new QLabel("标 识 符");
        id_label->setObjectName("idpwLabel");
        id_label->setAlignment(Qt::AlignCenter);
        id_input = new QLineEdit();
        idLayout->addWidget(id_label, 1);
        idLayout->addWidget(id_input, 3);

        QHBoxLayout* pwLayout = new QHBoxLayout(mainWidget); // password layout
        pw_label = new QLabel("密 　 码");
        pw_label->setObjectName("idpwLabel");
        pw_label->setAlignment(Qt::AlignCenter);
        pw_input = new QLineEdit();
        pw_input->setEchoMode(QLineEdit::Password);
        pwLayout->addWidget(pw_label, 1);
        pwLayout->addWidget(pw_input, 3);

        QHBoxLayout* btnLayout = new QHBoxLayout(mainWidget); // button layout
        go_btn = new QPushButton("确　认");
        go_btn->setObjectName("goBtn");
        go_btn->setProperty("btnStyle", "primary");
        go_btn->setCursor(Qt::PointingHandCursor);
        go_btn->setFixedWidth(120);
        go_btn->setFocusPolicy(Qt::NoFocus);
        cancel_btn = new QPushButton("取　消");
        cancel_btn->setObjectName("cancelBtn");
        cancel_btn->setProperty("btnStyle", "primary");
        cancel_btn->setCursor(Qt::PointingHandCursor);
        cancel_btn->setFixedWidth(120);
        cancel_btn->setFocusPolicy(Qt::NoFocus);
        btnLayout->addWidget(go_btn, 1);
        btnLayout->addWidget(cancel_btn, 1);

        lang_btn = new SlideButton("　　Lang: En."); // language switching slider
        lang_btn->setCursor(Qt::PointingHandCursor);
        lang_btn->setFixedWidth(120);
        lang_btn->setSwitchColors({
            QColor(107, 196, 238), // rgb(107, 196, 238)
            QColor(72, 239, 136), // rgb(72, 239, 136)
            QColor(255, 255, 255), // rgb(255, 255, 255)
            QColor(255, 255, 255) // rgb(255, 255, 255)
            });
        lang_btn->setThumbRadius(14);
        lang_btn->setTrackHeight(24);

        idLayout->setAlignment(Qt::AlignCenter);
        pwLayout->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(uiTitle);
        mainLayout->addStretch(1);
        mainLayout->addLayout(idLayout);
        mainLayout->addStretch(1);
        mainLayout->addLayout(pwLayout);
        mainLayout->addStretch(1);
        mainLayout->addLayout(btnLayout);
        mainLayout->addStretch(1);
        mainLayout->addWidget(lang_btn);
        mainLayout->setAlignment(lang_btn, Qt::AlignCenter);

        drag_sphere = new DragWidget(mainWindow, mainWidget);
        drag_sphere->setFixedSize(16, 16);
        drag_sphere->setAttribute(Qt::WA_StyledBackground, true);
        drag_sphere->setStyleSheet(
            "background: transparent;"
            "image: url(:/resources/drag-dots.svg);"
        );
        drag_sphere->raise();
        QMargins mainMargin = mainWidget->contentsMargins();
        drag_sphere->move(mainMargin.left() + 16, mainMargin.top() + 16);

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(mainWidget);
        shadow->setBlurRadius(BLUR_RADIUS);
        shadow->setOffset(1, 3);
        shadow->setColor(QColor(0, 0, 0, 80));
        mainWidget->setAttribute(Qt::WA_StyledBackground, true);
        mainWidget->setGraphicsEffect(shadow);
    }
}; // namespace Ui