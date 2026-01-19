#include "mainwindow.h"
#include <QtCore/qnamespace.h>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QWidget>
#include "locker.h"
#include "ui.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // 去掉系统矩形边框，原为 Qt::Window
    this->setAttribute(Qt::WA_TranslucentBackground); // 透明背景

    QString bin_path = QDir(QCoreApplication::applicationDirPath()).filePath("resources/core.bin");
    authenticator = new Authenticator(bin_path, this);

    connect(ui->id_input, &QLineEdit::returnPressed, ui->go_btn, &QPushButton::click);
    connect(ui->pw_input, &QLineEdit::returnPressed, ui->go_btn, &QPushButton::click);
    connect(ui->prot_btn, &QPushButton::clicked, this, &MainWindow::onProtBtnClicked);
    connect(ui->go_btn, &QPushButton::clicked, this, &MainWindow::onGoBtnClicked);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &MainWindow::onCancelBtnClicked);
    connect(ui->lang_btn, &QPushButton::toggled, this, [this]() {
        _isEn = !_isEn;
        refreshTexts();
        });
    connect(authenticator, &Authenticator::autSuccess, this, &MainWindow::onAutSuccess);
    connect(authenticator, &Authenticator::autFailed, this, &MainWindow::onAutFailed);
    connect(authenticator, &Authenticator::autPath, this, &MainWindow::onAutPathGet);
}

MainWindow::~MainWindow() {
    delete ui;
}

/// @brief 切换语言时刷新文字
void MainWindow::refreshTexts() {
    if (_isEn) {
        ui->uiTitle->setText("Protected Folders\nAccess Authentication");
        ui->id_label->setText("Identifier");
        ui->pw_label->setText("Password");
        ui->prot_btn->setText("Protect");
        ui->go_btn->setText("Confirm");
        ui->cancel_btn->setText("Cancel");
        ui->lang_btn->setText("语言：中　　");
    }
    else {
        ui->uiTitle->setText("受保护文件夹访问认证");
        ui->id_label->setText("标 识 符");
        ui->pw_label->setText("密 　 码");
        ui->prot_btn->setText("保　护");
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

void MainWindow::onProtBtnClicked() {
    QString id_str = ui->id_input->text();
    if (id_str.isEmpty()) {
        QMessageBox::critical(this, "Error", "Identifier cannot be empty!\n标识符不能为空！");
        return;
    }
    emit authenticator->searchPath(id_str);
}

void MainWindow::onGoBtnClicked() {
    QString id_str = ui->id_input->text();
    QString pw_str = ui->pw_input->text();
    emit authenticator->authenticate(id_str, pw_str);
}

void MainWindow::onCancelBtnClicked() {
    int ms = 200;
    fadeOutUI(ms);
    QTimer::singleShot(ms + 20, this, &MainWindow::close);
}

void MainWindow::onAutSuccess(const QString& folderPath) {
    QString dbPath = folderPath + ".db";
    if (!QFile::exists(dbPath)) {
        QMessageBox::warning(this, "Warning",
            "Folder is not protected or the path does not exist!\n文件夹未被保护，或路径不存在！"
        );
        return;
    }
    if (!QDir(folderPath).exists()) {
        if (!db2folder(dbPath, folderPath)) {
            QMessageBox::critical(this, "Error", "Failed to restore folder!\n还原文件夹失败！");
            return;
        }
        QFile::remove(dbPath);
    }
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath)))
        QMessageBox::critical(this, "Fail to open", "Verification error!\n校验错误！");
    else
        QMessageBox::information(this, "Tips",
            "To ensure the safety of your data, please click \"Protect\" "
            "in time to protect the folder data after use.\n"
            "为确保您的数据安全，使用完毕请及时点击 “保护” 来保护文件夹数据。"
        );
}

void MainWindow::onAutFailed(const QString& reason) {
    QMessageBox::critical(this, "Authenticate Failed", reason);
}

void MainWindow::onAutPathGet(const QString& folderPath) {
    if (folderPath.isEmpty())
        return;
    QFileInfo folderInfo(folderPath);
    QString dbPath = folderInfo.absolutePath() + "/" + folderInfo.fileName() + ".db";
    if (QFile::exists(dbPath)) {
        QMessageBox::warning(this, "Warning", "Folder is already protected!\n文件夹已被保护！");
        return;
    }
    if (!folder2db(folderPath, dbPath)) {
        QMessageBox::critical(this, "Error", "Failed to protect folder!\n文件夹保护失败！");
        return;
    }
    QDir dir(folderPath);
    if (!dir.removeRecursively()) {
        QMessageBox::warning(this, "Warning",
            "Failed to delete individual files due to occupation! "
            "To ensure data security, please delete the original folder manually.\n"
            "个别文件因占用而删除失败！为确保数据安全，请手动删除原文件夹。\n"
            "Otherwise, the next time you visit the folder, "
            "you won't be shown all the files until you manually delete the incomplete folder.\n"
            "否则，下次访问该文件夹时，将不会为您展示全部文件，直到您手动删除。\n"
            "Folder Path: " + folderPath
        );
    }
    QMessageBox::information(this, "Success",
        "Folder has been protected successfully.\n文件夹已被保护。"
    );
}