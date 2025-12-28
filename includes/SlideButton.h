#ifndef SLIDEBUTTON_H
#define SLIDEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QPropertyAnimation>

class SlideButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(qreal slidePosition READ slidePosition WRITE setSlidePosition)

public:
    explicit SlideButton(const QString &text = "", QWidget *parent = nullptr);
    void setCheckedAnimated(bool checked);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private slots:
    void onAnimationFinished(); // 动画结束槽函数
    
private:
    qreal m_slidePosition = 0.0;
    QPropertyAnimation *m_animation;
    
    qreal slidePosition() const { return m_slidePosition; }
    void setSlidePosition(qreal pos);
    
signals:
    void slidePositionChanged();
};

#endif