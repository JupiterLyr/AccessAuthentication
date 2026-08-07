#include "mainwindow.h"
#include <QtCore/qnamespace.h>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStorageInfo>
#include <QTimer>
#include <QWidget>
#include "ui.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    proc = new Processor(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // 去掉系统矩形边框，原为 Qt::Window
    this->setAttribute(Qt::WA_TranslucentBackground); // 透明背景

    QString bin_path = QDir(QCoreApplication::applicationDirPath()).filePath("resources/core.bin");
    authenticator = new Authenticator(bin_path, this);

    reporter = new QMessageBox(this);
    reporter->setStandardButtons(QMessageBox::NoButton);
    reporter->setWindowFlags(reporter->windowFlags() & ~Qt::WindowCloseButtonHint);

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

    connect(proc, &Processor::protectProgress, this, &MainWindow::onProgress);
    connect(proc, &Processor::restoreProgress, this, &MainWindow::onProgress);
    connect(proc, &Processor::protectFinished, this, &MainWindow::onProtectFinished);
    connect(proc, &Processor::restoreFinished, this, &MainWindow::onRestoreFinished);
    connect(proc, &Processor::reportReceived, this, &MainWindow::showReport);
}

MainWindow::~MainWindow() {
    delete ui;
    delete proc;
}

void MainWindow::btn_enable() {
    ui->go_btn->setEnabled(true);
    ui->prot_btn->setEnabled(true);
    ui->cancel_btn->setEnabled(true);
}

void MainWindow::btn_disable() {
    ui->go_btn->setDisabled(true);
    ui->prot_btn->setDisabled(true);
    ui->cancel_btn->setDisabled(true);
}

/// @brief 切换语言时刷新文字
void MainWindow::refreshTexts() {
    if (_isEn) {
        ui->uiTitle->setText("Protected Folders\nAccess Authentication");
        ui->id_label->setText("Identifier");
        ui->pw_label->setText("Password");
        ui->prot_btn->setText("Protect");
        ui->go_btn->setText("Unlock");
        ui->cancel_btn->setText("Close");
        ui->lang_btn->setText("语言：中　　");
    }
    else {
        ui->uiTitle->setText("受保护文件夹访问认证");
        ui->id_label->setText("标 识 符");
        ui->pw_label->setText("密 　 码");
        ui->prot_btn->setText("保　护");
        ui->go_btn->setText("解　锁");
        ui->cancel_btn->setText("关　闭");
        ui->lang_btn->setText("　　Lang: En.");
    }
}

/// @brief 界面渐隐
/// @param duration 持续时间 (ms)
void MainWindow::fadeOutUI(int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
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
    btn_disable();
    emit authenticator->searchPath(id_str);
}

void MainWindow::onGoBtnClicked() {
    QString id_str = ui->id_input->text();
    QString pw_str = ui->pw_input->text();
    btn_disable();
    emit authenticator->authenticate(id_str, pw_str);
}

void MainWindow::onCancelBtnClicked() {
    auto ret = QMessageBox::question(this, "Exit",
        "Do you want to quit?\n您确定要退出吗？",
        QMessageBox::Yes | QMessageBox::No
    );
    if (ret == QMessageBox::No)
        return;
    int ms = 200;
    fadeOutUI(ms);
    QTimer::singleShot(ms + 20, this, &MainWindow::close);
}

void MainWindow::onAutSuccess(const QString& folderPath) {
    QString _folderpath = folderPath;
    if (folderPath.startsWith('Z')) {  // 盘符重定向
        QStorageInfo appDir(QCoreApplication::applicationDirPath());
        QString drive_letter = appDir.rootPath().left(1);
        _folderpath.replace(0, 1, drive_letter);
    }
    QString dbPath = _folderpath + ".db";
    if (!QFile::exists(dbPath)) {
        QMessageBox::warning(this, "Warning",
            "Folder is not protected or the path does not exist!\n文件夹未被保护，或路径不存在！"
        );
        btn_enable();
        return;
    }
    if (!QDir(_folderpath).exists()) {
        pgDialog = new QProgressDialog("Processing...", "Cancel", 0, 100, this);
        pgDialog->setWindowTitle("Restoration");
        pgDialog->setWindowModality(Qt::WindowModal);
        pgDialog->setMinimumDuration(500);  // 高于500ms才弹出
        pgDialog->setValue(0);
        connect(pgDialog, &QProgressDialog::canceled, proc, &Processor::cancelTask);
        pgDialog->show();
        proc->restoreFolder(dbPath, _folderpath);
    }
    else {
        QMessageBox::critical(this, "Error", "There is a folder with the same name!\n当前存在同名文件夹！");
        btn_enable();
    }
}

void MainWindow::onAutFailed(const QString& reason) {
    QMessageBox::critical(this, "Authenticate Failed", reason);
    btn_enable();
}

void MainWindow::onAutPathGet(const QString& folderPath) {
    if (folderPath.isEmpty()) // 传入空字符串
        return;
    QString _folderpath = folderPath;
    if (folderPath.startsWith('Z')) {  // 对所谓 Z: 盘重定向到软件所在盘符（仅windows系统适用）
        QStorageInfo appDir(QCoreApplication::applicationDirPath());
        QString drive_letter = appDir.rootPath().left(1);
        _folderpath.replace(0, 1, drive_letter);
    }
    QFileInfo folderInfo(_folderpath);
    QString dbPath = folderInfo.absolutePath() + "/" + folderInfo.fileName() + ".db";
    if (QFile::exists(dbPath)) {
        QMessageBox::warning(this, "Warning", "Folder is already protected!\n文件夹已被保护！");
        btn_enable();
        return;
    }
    if (!folderInfo.exists() || !folderInfo.isDir()) {
        QMessageBox::critical(this, "Error", "Folder not found!\n文件夹不存在！");
        btn_enable();
        return;
    }
    pgDialog = new QProgressDialog("Processing...", "Cancel", 0, 100, this);
    pgDialog->setWindowTitle("Protection");
    pgDialog->setWindowModality(Qt::WindowModal);
    pgDialog->setMinimumDuration(500);  // 高于500ms才弹出
    pgDialog->setValue(0);
    connect(pgDialog, &QProgressDialog::canceled, proc, &Processor::cancelTask);
    pgDialog->show();
    proc->protectFolder(_folderpath, dbPath);
}

void MainWindow::onProgress(quint64 done, quint64 total) {
    if (!pgDialog || total == 0)
        return;
    int percent = static_cast<int>((done * 100) / total);
    pgDialog->setValue(percent);
}

void MainWindow::onProtectFinished(int condition, QString message) {
    if (reporter && reporter->isVisible())
        reporter->hide();
    if (condition < 10)
        QMessageBox::information(this, "Success", message);
    else if (condition >= 70)
        QMessageBox::warning(this, "Warning", message);
    else
        QMessageBox::critical(this, "Error", message);
    if (pgDialog) {
        pgDialog->setValue(100);
        pgDialog->close();
        pgDialog->deleteLater();
        pgDialog = nullptr;
    }
    btn_enable();
}

void MainWindow::onRestoreFinished(int condition, QString message) {
    if (reporter && reporter->isVisible())
        reporter->hide();
    if (condition == 0) {  // 若成功，message 实际上是 folderPath
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(message)))
            QMessageBox::critical(this, "Fail to open", "Verification error!\n校验错误！");
        else
            QMessageBox::information(this, "Tips",
                "To ensure the safety of your data, please click \"Protect\" "
                "in time to protect the folder data after use.\n"
                "为确保您的数据安全，使用完毕请及时点击 “保护” 来保护文件夹数据。"
            );
    }
    else if (condition < 10)
        QMessageBox::information(this, "Success", message);
    else if (condition >= 70)
        QMessageBox::warning(this, "Warning", message);
    else
        QMessageBox::critical(this, "Error", message);
    if (pgDialog) {
        pgDialog->setValue(100);
        pgDialog->close();
        pgDialog->deleteLater();
        pgDialog = nullptr;
    }
    btn_enable();
}

/// @param type 0: info | 1: warning | 2: error
void MainWindow::showReport(int type, QString message) {
    switch (type) {
    case 0:
        reporter->setIcon(QMessageBox::Information);
        reporter->setWindowTitle("Report");
        break;
    case 1:
        reporter->setIcon(QMessageBox::Warning);
        reporter->setWindowTitle("Warning");
        break;
    case 2:
        reporter->setIcon(QMessageBox::Critical);
        reporter->setWindowTitle("Error");
        break;
    default:
        reporter->setIcon(QMessageBox::Critical);
        reporter->setWindowTitle("Unknown Error");
        message = (QString("Unknown Error!\nCode: %1\n%2").arg(type).arg(message));
    }
    reporter->setText(message);
    if (!reporter->isVisible())
        reporter->show();
    else
        reporter->raise();
}