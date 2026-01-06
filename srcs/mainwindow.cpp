#include "mainwindow.h"
#include <QtCore/qnamespace.h>
#include <QTimer>
#include <QMessageBox>
#include <QWidget>
#include "ui.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // 去掉系统矩形边框，原为 Qt::Window
    this->setAttribute(Qt::WA_TranslucentBackground); // 透明背景

    connect(ui->id_input, &QLineEdit::returnPressed, ui->go_btn, &QPushButton::click);
    connect(ui->pw_input, &QLineEdit::returnPressed, ui->go_btn, &QPushButton::click);
    connect(ui->go_btn, &QPushButton::clicked, this, &MainWindow::onGoBtnClicked);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &MainWindow::onCancelBtnClicked);
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
        ui->id_label->setText("标 识 符");
        ui->pw_label->setText("密 　 码");
        ui->go_btn->setText("确　认");
        ui->cancel_btn->setText("取　消");
        ui->lang_btn->setText("　　Lang: En.");
    }
}

/// @brief 界面渐隐
/// @param duration 持续时间 (ms)
void MainWindow::fadeOutUI(int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(duration);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);  // 完成后自动删除
}

void MainWindow::onGoBtnClicked() {
    if (_isEn)
        QMessageBox::warning(this, "Warning", "This function has not been developed yet!");
    else
        QMessageBox::warning(this, "注意", "该功能尚未开发！");
}

void MainWindow::onCancelBtnClicked() {
    int ms = 200;
    fadeOutUI(ms);
    QTimer::singleShot(ms + 20, this, &MainWindow::close);
}