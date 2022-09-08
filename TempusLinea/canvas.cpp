#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget *parent) : QWidget{parent}
{
    //Setup default parameters
    range_begin = 1950 * 12 * 31;
    range_end = 2050 * 12 * 31;
}

void Canvas::paintEvent(QPaintEvent *)
{
    v_offset = height() / 2;

    QPainter painter(this);

    QColor timeline_color(0, 0,0);

    // Paint Timeline
    painter.setPen(timeline_color);
    painter.drawLine(0, v_offset, width(), v_offset);
}
