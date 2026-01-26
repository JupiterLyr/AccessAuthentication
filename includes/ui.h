#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "DragWidget.h"
#include "HelpDialog.h"
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
        QPushButton* prot_btn;
        QPushButton* go_btn;
        QPushButton* cancel_btn;
        SlideButton* lang_btn;

        DragWidget* drag_sphere; // 拖拽部件
        HelpButton* help_btn;
    };
}

#endif // UI_H