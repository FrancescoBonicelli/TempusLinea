#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget *parent) : QWidget{parent}
{
    //Setup default parameters
    range_begin = 1950 * 12 * 31;
    range_end = 2050 * 12 * 31;

    v_offset = 0;
    dragging = false;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QColor timeline_color(0, 0,0);

    // Paint Timeline
    painter.setPen(timeline_color);
    painter.drawLine(0, (height() / 2) + v_offset, width(), (height() / 2) + v_offset);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        starting_drag_position = event->pos();
        dragging = true;
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        dragging = false;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if(dragging)
    {
        v_offset += event->pos().y() - starting_drag_position.y();
        starting_drag_position = event->pos();
        update();
    }
}

