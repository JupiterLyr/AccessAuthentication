#include "DragWidget.h"
#include <QMouseEvent>

/// @warning 使用时必须绑定一个父组件
DragWidget::DragWidget(QWidget* window, QWidget* parent)
    : QWidget(parent), m_window(window) {
    setCursor(Qt::SizeAllCursor);
}

void DragWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_window) {
        m_dragOffset = event->globalPosition().toPoint() - m_window->frameGeometry().topLeft();
    }
}

void DragWidget::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && m_window) {
        m_window->move(event->globalPosition().toPoint() - m_dragOffset);
    }
}