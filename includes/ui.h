#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "DragWidget.h"
#include "SlideButton.h"

namespace Ui {
    class MainWindow {
    public:
        void setupUi(QMainWindow* mainWindow);

        QLabel* uiTitle;
        QLabel* id_label;
        QLineEdit* id_input;
        QLabel* pw_label;
        QLineEdit* pw_input;
        QPushButton* go_btn;
        QPushButton* cancel_btn;
        SlideButton* lang_btn;

        DragWidget* drag_sphere; // 拖拽部件
    };
}

#endif // UI_H