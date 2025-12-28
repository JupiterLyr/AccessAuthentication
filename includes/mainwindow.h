#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>

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
};
#endif // MAINWINDOW_H
