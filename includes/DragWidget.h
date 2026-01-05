#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H
#pragma once

#include <QWidget>
#include <QPoint>

class DragWidget : public QWidget {
    Q_OBJECT
public:
    explicit DragWidget(QWidget* window, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QWidget* m_window = nullptr;
    QPoint m_dragOffset;
};

#endif