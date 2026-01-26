#ifndef HELP_H
#define HELP_H

#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>

/// @brief 显示帮助窗口
class HelpDialog : public QDialog {
    Q_OBJECT

public:
    explicit HelpDialog(QWidget* parent = nullptr);

private:
    QTextBrowser* m_textBrowser;
};

/// @brief 唤起帮助按钮
class HelpButton : public QPushButton {
    Q_OBJECT
public:
    explicit HelpButton(QWidget* parent = nullptr);

private slots:
    void onClicked();
};

#endif