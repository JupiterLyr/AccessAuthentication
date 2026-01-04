#ifndef SLIDEBUTTON_H
#define SLIDEBUTTON_H

#include <QObject>
#include <QColor>
#include <QPushButton>
#include <QPropertyAnimation>

class SlideButton : public QPushButton {
    Q_OBJECT
        Q_PROPERTY(qreal slidePosition READ slidePosition WRITE setSlidePosition NOTIFY slidePositionChanged)

public:
    /** @brief Slide Button Widget Color Struction:
     * ``trackOff``, ``trackOn``, ``thumbOff``, ``thumbOn``
     */
    struct SwitchColors {
        QColor trackOff;
        QColor trackOn;
        QColor thumbOff;
        QColor thumbOn;
    };

    explicit SlideButton(const QString& text = "", QWidget* parent = nullptr);

    void setSwitchColors(const SwitchColors& colors);

    void setThumbRadius(int r) { m_thumbRadius = r; updateGeometry(); update(); } // 设置滑块半径
    void setTrackHeight(int h) { m_trackHeight = h; updateGeometry(); update(); } // 设置滑轨高度
    void setAnimationDuration(int ms) { m_animation->setDuration(ms); } // 动画时长设置 (default: 300)

    QSize sizeHint() const override;

    qreal slidePosition() const { return m_slidePosition; }

public slots:
    void setSlidePosition(qreal pos);
    void toggleAnimated();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void onAnimationFinished();

private:
    qreal m_slidePosition = 0.0; // 0 - left(off), 1 - right(on)
    qreal m_lastPosition = 0.0; // 上次位置
    QPropertyAnimation* m_animation;

    SwitchColors m_colors; // 组件颜色

    int m_thumbRadius = 13;
    int m_trackHeight = 22;
    int m_margin = 6; // 轨道与控件边缘的内距

    // 拖拽控制
    bool m_dragging = false;
    QPoint m_pressPos;
    qreal m_pressSlidePos = 0.0;

signals:
    void slidePositionChanged();
};

#endif // SLIDEBUTTON_H