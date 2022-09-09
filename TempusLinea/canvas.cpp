#include "canvas.h"

#include <iostream>

int years2Days(int years)
{
    return years * 12 * 31;
}

Canvas::Canvas(QWidget *parent) : QWidget{parent}
{
    //Setup default parameters
    range_begin = years2Days(1949);
    range_end = years2Days(2049);

    v_offset = 0;
    dragging = false;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QColor timeline_color(0, 0,0);

    // Paint Timeline
    painter.setPen(timeline_color);
    int y = (height() / 2) + v_offset;
    painter.drawLine(0, y, width(), y);

    // Compute the timelline ticks
    int i = years2Days(1);
    while ((range_end - range_begin) / i > width() / 40)
    {
        i *= 5;
    }

    for (int tick = -1; tick <= (range_end - range_begin) / i + 1; tick++)
    {
        int x = width() * (tick * i - range_begin % i) / (range_end - range_begin);
        painter.drawLine(x, y + 5, x, y - 5);
        painter.drawText(x, y - 10, std::to_string((tick * i - range_begin % i + range_begin) / (years2Days(1))).data());
    }
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
        range_begin -= (event->pos().x() - starting_drag_position.x()) * (range_end - range_begin) / width();
        range_end -= (event->pos().x() - starting_drag_position.x()) * (range_end - range_begin) / width();
        starting_drag_position = event->pos();
        update();
    }
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    int delta_x = event->angleDelta().y() * (range_end - range_begin) / width();

    // Reducing range -> min 5 years
    if (delta_x > 0 && range_end - range_begin - delta_x < years2Days(5))
    {
        delta_x = (range_end - range_begin) - years2Days(5);
    }
    // Enlarging range -> max 2000 years
    else if (delta_x < 0 && range_end - range_begin - delta_x > years2Days(2000))
    {
        delta_x = (range_end - range_begin) - years2Days(2000);
    }

    float cursor_position = (float)event->position().x() / width();
    range_begin += delta_x * cursor_position;
    range_end -= delta_x * (1 - cursor_position);
    update();
}