#include "mainwindow.h"
#include <QtCore/qnamespace.h>
#include <QWidget>
#include "ui.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // 去掉系统矩形边框，原为 Qt::Window
    this->setAttribute(Qt::WA_TranslucentBackground); // 允许透明背景，实现圆角

    connect(ui->cancel_btn, &QPushButton::clicked, this, [this]() {this->close();});
    connect(ui->lang_btn, &QPushButton::toggled, this, [this]() {
        _isEn = !_isEn;
        refreshTexts();
        });
}

MainWindow::~MainWindow() {
    delete ui;
}

/// @brief 切换语言时刷新文字
void MainWindow::refreshTexts() {
    if (_isEn) {
        ui->uiTitle->setText("Protected Files\nAccess Authentication");
        ui->id_label->setText("Identifier");
        ui->pw_label->setText("Password");
        ui->go_btn->setText("Confirm");
        ui->cancel_btn->setText("Cancel");
        ui->lang_btn->setText("语言：中　　");
    }
    else {
        ui->uiTitle->setText("受保护文件访问认证");
        ui->id_label->setText("标识符");
        ui->pw_label->setText("密码");
        ui->go_btn->setText("确认");
        ui->cancel_btn->setText("取消");
        ui->lang_btn->setText("　　Lang: En.");
    }
}
