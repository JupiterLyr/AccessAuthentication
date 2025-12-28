#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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
        SlideButton* lang_btn;
    };
}

#endif // UI_H