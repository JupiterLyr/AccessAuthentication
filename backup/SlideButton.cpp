#include "SlideButton.h"
#include <QPainter>
#include <QStyleOptionButton>

SlideButton::SlideButton(const QString& text, QWidget* parent) : QPushButton(text, parent) {
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    // setFixedWidth(150);
    __slidePosition = 0.0;
    __animation = new QPropertyAnimation(this, "slidePosition", this);
    __animation->setDuration(500); // 动画时长 (ms)
    __animation->setEasingCurve(QEasingCurve::InOutCubic);

    connect(__animation, &QPropertyAnimation::finished, this, &SlideButton::onAnimationFinished);
}

void SlideButton::setColors(const QColor& darkColor, const QColor& lightColor) {
    __darkColor = darkColor;
    __lightColor = lightColor;
    update();
}

void SlideButton::setDarkColor(const QColor& color) {
    __darkColor = color;
    update();
}

void SlideButton::setLightColor(const QColor& color) {
    __lightColor = color;
    update();
}

void SlideButton::toggleAnimated() {
    if (__animation->state() == QPropertyAnimation::Running)
        __animation->stop();
    __lastPosition = __slidePosition;  // 改变状态前保存原状态
    bool targetChecked = !isChecked(); // 目标状态
    qreal endPos = targetChecked ? 1.0 : 0.0;
    __animation->setStartValue(__lastPosition);
    __animation->setEndValue(endPos);
    __animation->start();
}

/// @brief 绘画事件
void SlideButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    qreal slide = __slidePosition;
    // 绘制两个半区
    int splitX = rect.left() + rect.width() * slide;
    // 左侧区域
    QColor leftColor = QColor::fromRgbF(
        __darkColor.redF()   * (1 - slide) + __lightColor.redF()   * slide,
        __darkColor.greenF() * (1 - slide) + __lightColor.greenF() * slide,
        __darkColor.blueF()  * (1 - slide) + __lightColor.blueF()  * slide
    );
    painter.fillRect(rect.left(), rect.top(), splitX - rect.left(), rect.height(), leftColor);
    // 右侧区域
    QColor rightColor = QColor::fromRgbF(
        __lightColor.redF()   * (1 - slide) + __darkColor.redF()   * slide,
        __lightColor.greenF() * (1 - slide) + __darkColor.greenF() * slide,
        __lightColor.blueF()  * (1 - slide) + __darkColor.blueF()  * slide
    );
    painter.fillRect(splitX, rect.top(), rect.right() - splitX + 1, rect.height(), rightColor);
    // 绘制分割线
    painter.setPen(QColor(255, 255, 255, 120));
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

/// @brief 设定滑块位置
void SlideButton::setSlidePosition(qreal pos) {
    __slidePosition = pos;
    update(); // 触发重绘
}

/// @brief 动画结束
void SlideButton::onAnimationFinished() {
    QPushButton::setChecked(isChecked());
    update();
}