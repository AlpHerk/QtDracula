#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>

class TitleBar : public QLabel
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

protected:
     //用于鼠标拖动窗口的鼠标事件操作
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:

};

#endif // TITLEBAR_H
