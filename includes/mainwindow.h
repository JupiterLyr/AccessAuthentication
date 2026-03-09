#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include "authenticator.h"
#include "processor.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void refreshTexts();
    bool _isEn = false;

private:
    Ui::MainWindow* ui;
    Authenticator* authenticator;
    Processor* proc;  // 线程管理

private slots:
    void fadeOutUI(int duration = 100);
    void onProtBtnClicked();
    void onGoBtnClicked();
    void onCancelBtnClicked();
    void onAutSuccess(const QString& folderPath);
    void onAutFailed(const QString& reason);
    void onAutPathGet(const QString& folderPath);
};
#endif // MAINWINDOW_H
