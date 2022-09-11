#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    //canvas_start = date(1949, 1, 1);
    //canvas_end = date(2049, 1, 1);
    canvas_start = date(6949, 1, 1);
    canvas_end = date(7049, 1, 1);

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
    int i = 1;
    while ((canvas_end.year() - canvas_start.year()) / i > width() / 40)
    {
        // TODO: find a better scaling logic, I don't like 1.5.25.125, 1.5.10.20.50.100 is better
        i *= 5;
    }

    // Draw the timeline ticks
    int x_span = (canvas_end-canvas_start).days();
    date first_tick_date(canvas_start.year() - (canvas_start.year() % i), 1, 1);
    for (date d = first_tick_date; d <= canvas_end; d += years(i))
    {
        int x = width() * (d - canvas_start).days() / x_span;
        painter.drawLine(x, y + 5, x, y - 5);
        painter.drawText(x, y - 10, std::to_string(d.year()).data());
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
        canvas_start -= days((event->pos().x() - starting_drag_position.x()) * (canvas_end - canvas_start).days() / width());
        canvas_end -= days((event->pos().x() - starting_drag_position.x()) * (canvas_end - canvas_start).days() / width());
        starting_drag_position = event->pos();
        update();
    }
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    int delta_x = event->angleDelta().y() * (canvas_end - canvas_start).days() / width();

    // Reducing range -> min 5 years
    if (delta_x > 0 && canvas_end - years(5) < canvas_start + days(delta_x))
    {
        delta_x = (canvas_end - (canvas_start + years(5))).days();
    }
    // Enlarging range -> max 2000 years
    else if (delta_x < 0 && canvas_end - years(2000) > canvas_start + days(delta_x))
    {
        delta_x = (canvas_end - (canvas_start + years(2000))).days();
    }

    float cursor_position = (float)event->position().x() / width();
    canvas_start += days(delta_x * cursor_position);
    canvas_end -= days(delta_x * (1 - cursor_position));
    update();
}