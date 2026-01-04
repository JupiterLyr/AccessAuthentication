#include "mainwindow.h"
#include <QtCore/qnamespace.h>
#include <QWidget>
#include "ui.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window); // 去掉系统矩形边框
    this->setAttribute(Qt::WA_TranslucentBackground); // 允许透明背景，实现圆角

    connect(ui->lang_btn, &QPushButton::toggled, this, [this]() {
        _isEn = !_isEn;
        refreshTexts();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

/// @brief 刷新文字
void MainWindow::refreshTexts() {
    if (_isEn) {
        ui->uiTitle->setText("Protected Files\nAccess Authentication");
        ui->id_label->setText("Identifier");
        ui->pw_label->setText("Password");
        ui->go_btn->setText("Confirm");
        ui->lang_btn->setText("简体中文　　");
    }
    else {
        ui->uiTitle->setText("受保护文件访问认证");
        ui->id_label->setText("标识符");
        ui->pw_label->setText("密码");
        ui->go_btn->setText("确认");
        ui->lang_btn->setText("　　English");
    }
}
