#include "SlideButton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionButton>

SlideButton::SlideButton(const QString& text, QWidget* parent) : QPushButton(text, parent) {
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    // setFixedWidth(150);
    m_animation = new QPropertyAnimation(this, "slidePosition", this);
    m_animation->setDuration(800); // 动画时长 (ms)
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);

    // 动画结束信号
    connect(m_animation, &QPropertyAnimation::finished, this, &SlideButton::onAnimationFinished);
}

void SlideButton::setCheckedAnimated(bool checked) {
    if (checked == isChecked() && m_animation->state() != QPropertyAnimation::Running)
        return;
    if (m_animation->state() == QPropertyAnimation::Running)
        m_animation->stop();
    m_animation->setStartValue(checked ? 0.0 : 1.0);
    m_animation->setEndValue(checked ? 1.0 : 0.0);
    QPushButton::setChecked(checked);
    m_animation->start();
}

/// @brief 绘画事件
void SlideButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    qreal slide = m_slidePosition;
    QColor leftColor, rightColor;
    if (isChecked()) {
        // 选中状态：左浅色，右深色
        leftColor = QColor(216, 241, 245);  // 浅色
        rightColor = QColor(33, 150, 243);  // 深色 #2196F3
    }
    else {
        // 未选中：左深色，右浅色
        leftColor = QColor(33, 150, 243);   // 深色
        rightColor = QColor(216, 241, 245); // 浅色
    }
    // 绘制两个半区
    int splitX = rect.left() + rect.width() * slide;
    // 左侧区域
    painter.fillRect(rect.left(), rect.top(), splitX - rect.left(), rect.height(), leftColor);
    // 右侧区域
    painter.fillRect(splitX, rect.top(), rect.right() - splitX + 1, rect.height(), rightColor);
    // 绘制分割线
    painter.setPen(QColor(255, 255, 255, 100));
    painter.drawLine(splitX, rect.top(), splitX, rect.bottom());
    // 绘制文字
    painter.setPen(Qt::black);
    painter.setFont(font());

    QString text = this->text();
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text);
    int textX = (rect.width() - textWidth) / 2;
    int textY = (rect.height() + fm.ascent() - fm.descent()) / 2;

    painter.drawText(textX, textY, text);
}

void SlideButton::setSlidePosition(qreal pos) {
    m_slidePosition = pos;
    update(); // 触发重绘
    emit slidePositionChanged();
}

/// @brief 动画结束
void SlideButton::onAnimationFinished() {
    update();
}