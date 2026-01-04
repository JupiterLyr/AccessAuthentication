#include "SlideButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOption>

/// @brief 双色插值
/// @param a 颜色 1
/// @param b 颜色 2
/// @param t 位置参数
static QColor interpolate(const QColor& a, const QColor& b, qreal t) {
    t = qBound(0.0, t, 1.0);
    return QColor::fromRgbF(
        a.redF() * (1 - t) + b.redF() * t,
        a.greenF() * (1 - t) + b.greenF() * t,
        a.blueF() * (1 - t) + b.blueF() * t,
        a.alphaF() * (1 - t) + b.alphaF() * t
    );
}

SlideButton::SlideButton(const QString& text, QWidget* parent) : QPushButton(text, parent) {
    setCheckable(true);
    setFocusPolicy(Qt::NoFocus);
    m_slidePosition = isChecked() ? 1.0 : 0.0;

    m_animation = new QPropertyAnimation(this, "slidePosition", this);
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
    connect(m_animation, &QPropertyAnimation::finished, this, &SlideButton::onAnimationFinished);
}

/// @brief 配色设置
/// @param colors 按 ``trackOff``, ``trackOn``, ``thumbOff``, ``thumbOn`` 设置
void SlideButton::setSwitchColors(const SwitchColors& colors) {
    m_colors = colors;
    update();
}

QSize SlideButton::sizeHint() const {
    QFontMetrics fm(font());
    int w = qMax(80, fm.horizontalAdvance(text()) + 60); // 最小宽度
    int h = qMax(28, m_trackHeight + m_margin * 2);
    return QSize(w, h);
}

void SlideButton::setSlidePosition(qreal pos) {
    pos = qBound(0.0, pos, 1.0);
    if (qFuzzyCompare(pos + 1.0, m_slidePosition + 1.0))
        return;
    m_slidePosition = pos;
    update();
    emit slidePositionChanged();
}

void SlideButton::toggleAnimated() {
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    m_lastPosition = m_slidePosition;
    qreal endPos = isChecked() ? 0.0 : 1.0;

    m_animation->stop();
    m_animation->setStartValue(m_lastPosition);
    m_animation->setEndValue(endPos);
    m_animation->start();
} // 在 onAnimationFinished 中设置 checked，避免视觉与状态不同步

void SlideButton::onAnimationFinished() {
    bool checkedNow = m_slidePosition >= 0.5;
    if (isChecked() != checkedNow)
        QPushButton::setChecked(checkedNow);
    update();
}

// 鼠标左键点击事件
void SlideButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) { // 鼠标左键
        event->ignore();
        return;
    }
    // if (event->button() == Qt::LeftButton)
    //     toggleAnimated();
    // QPushButton::mousePressEvent(event); // WARNING!!!

    m_pressPos = event->pos();
    m_pressSlidePos = m_slidePosition;
    m_dragging = false;

    // 如果点击在滑块范围内，则准备拖拽；否则直接以点击方式切换
    QRect trackRect = rect().adjusted(m_margin, (rect().height() - m_trackHeight) / 2,
        -m_margin, -(rect().height() - m_trackHeight) / 2);

    // 计算滑块中心
    int usable = trackRect.width() - 2 * m_thumbRadius;
    int thumbX = trackRect.left() + int(m_slidePosition * usable) + m_thumbRadius;
    QPoint thumbCenter(thumbX, trackRect.center().y());
    int hitRadius = m_thumbRadius + 6;
    if ((event->pos() - thumbCenter).manhattanLength() <= hitRadius) {
        m_dragging = true;
        if (m_animation->state() == QPropertyAnimation::Running)
            m_animation->stop();
    }
    event->accept();
}

// 鼠标拖动事件
void SlideButton::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragging) {
        // QPushButton::mouseMoveEvent(event);
        return;
    }
    QRect trackRect = rect().adjusted(m_margin, (rect().height() - m_trackHeight) / 2,
        -m_margin, -(rect().height() - m_trackHeight) / 2);
    int usable = trackRect.width() - 2 * m_thumbRadius;
    if (usable <= 0) return;

    int dx = event->pos().x() - m_pressPos.x();
    qreal delta = qreal(dx) / qreal(usable);
    qreal newPos = qBound(0.0, m_pressSlidePos + delta, 1.0);
    setSlidePosition(newPos);
    event->accept();
}

// 鼠标释放事件
void SlideButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton)
        return;
    bool wasDragging = m_dragging, targetChecked;
    m_dragging = false;
    if (wasDragging) // 刚刚在拖拽
        targetChecked = m_slidePosition >= 0.5;
    else // 刚刚在单击
        targetChecked = !isChecked();

    m_animation->stop();
    m_animation->setStartValue(m_slidePosition);
    m_animation->setEndValue(targetChecked ? 1.0 : 0.0);
    m_animation->start();
    event->accept();
}

void SlideButton::paintEvent(QPaintEvent* /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    QRect trackRect( // 轨道矩形
        m_margin,
        (r.height() - m_trackHeight) / 2,
        r.width() - m_margin * 2,
        m_trackHeight
    );
    qreal tRadius = trackRect.height() / 2.0; // 轨道半径

    QColor trackColor = interpolate( // 轨道颜色
        m_colors.trackOff,
        m_colors.trackOn,
        m_slidePosition
    );
    QColor thumbColor = interpolate( // 滑块颜色
        m_colors.thumbOff,
        m_colors.thumbOn,
        m_slidePosition
    );

    QRectF shadowRect = QRectF(trackRect).adjusted(-1.5, -1.5, 1.5, 1.5);
    // 绘制轨道阴影
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 28));
    p.drawRoundedRect(shadowRect, tRadius + 1.5, tRadius + 1.5);

    // 绘制轨道
    p.setBrush(trackColor);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(trackRect, tRadius, tRadius);

    // 计算滑块中心位置
    int usable = trackRect.width() - 2 * m_thumbRadius;
    qreal x = trackRect.left() + m_thumbRadius + m_slidePosition * (usable < 0 ? 0 : usable);
    qreal y = trackRect.center().y();

    // 绘制滑块阴影
    QRectF glowRect(x - m_thumbRadius - 3.0, y - m_thumbRadius - 3.0, (m_thumbRadius + 3.0) * 2, (m_thumbRadius + 3.0) * 2);
    p.setBrush(QColor(0, 0, 0, 30));
    p.drawEllipse(glowRect);

    // 绘制滑块主体
    p.setBrush(thumbColor);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(x, y), m_thumbRadius, m_thumbRadius);

    // 绘制文字
    p.setPen(palette().buttonText().color());
    p.setFont(font());
    QFontMetrics fm(font());
    int textX = (r.width() - fm.horizontalAdvance(text())) / 2;
    int textY = (r.height() + fm.ascent() - fm.descent()) / 2;
    p.drawText(textX, textY, text());
}