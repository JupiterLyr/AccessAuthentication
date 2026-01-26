#include "HelpDialog.h"
#include <QFile>
#include <QFont>
#include <QVBoxLayout>

static QString loadMarkdown(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString("Failed to load `help.md`!");
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    return in.readAll();
}

HelpDialog::HelpDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Help");
    resize(720, 640);
    m_textBrowser = new QTextBrowser(this);
    m_textBrowser->setReadOnly(true);
    m_textBrowser->setFrameStyle(QFrame::NoFrame);
    m_textBrowser->setOpenExternalLinks(true); // 跳转链接
    m_textBrowser->setOpenLinks(true); // 内部锚点
    QFont font;
    font.setPointSize(11);  // 字体大小
    m_textBrowser->setFont(font);
    m_textBrowser->setMarkdown(loadMarkdown(":/resources/help.md"));
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_textBrowser);
}

HelpButton::HelpButton(QWidget* parent) : QPushButton(parent) {
    setToolTip("Help");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(
        "background: transparent;"
        "image: url(:/resources/help_icon.svg);"
    );
    connect(this, &QPushButton::clicked, this, &HelpButton::onClicked);
}

void HelpButton::onClicked() {
    HelpDialog* dlg = new HelpDialog();
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}