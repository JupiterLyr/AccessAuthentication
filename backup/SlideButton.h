#ifndef SLIDEBUTTON_H
#define SLIDEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QPropertyAnimation>

class SlideButton : public QPushButton {
    Q_OBJECT
        Q_PROPERTY(qreal slidePosition READ slidePosition WRITE setSlidePosition)

public:
    explicit SlideButton(const QString& text = "", QWidget* parent = nullptr);
    // 设置颜色（公开接口）
    void setColors(const QColor& darkColor, const QColor& lightColor);
    void setDarkColor(const QColor& color);
    void setLightColor(const QColor& color);
    void toggleAnimated();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAnimationFinished(); // 动画结束槽函数

private:
    qreal __slidePosition = 0.0;
    qreal __lastPosition = 0.0; // 上次位置
    QPropertyAnimation* __animation;

    QColor __darkColor = QColor(33, 150, 243);    // 默认深色
    QColor __lightColor = QColor(216, 241, 245);  // 默认浅色

    qreal slidePosition() const { return __slidePosition; }
    void setSlidePosition(qreal pos);
};

#endif