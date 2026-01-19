#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include "authenticator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT  // 需要用到信号和槽时要加回来

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void refreshTexts();
    bool _isEn = false;

private:
    Ui::MainWindow* ui;
    Authenticator* authenticator;

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
