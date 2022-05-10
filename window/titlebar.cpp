#include <QPointF>
#include <QMouseEvent>

#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent): QLabel{parent}
{

}

// 鼠标拖拽窗口事件 *************************************************
QPoint localOffPos;
bool   mouseMoving   = false;

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    QWidget* wdo = this->window();
    if (event->button() == Qt::LeftButton) {
         mouseMoving = true;
         localOffPos = event->globalPosition().toPoint() - wdo->pos();
    }
    return QLabel::mousePressEvent(event);
}
void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    QWidget* wdo = this->window();
    if (mouseMoving) {
         wdo->move(event->globalPosition().toPoint() - localOffPos);
    }
    return QLabel::mouseMoveEvent(event);
}
void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    mouseMoving = false;
    return QLabel::mouseReleaseEvent(event);
}
// 鼠标拖拽窗口事件 **********************************************
