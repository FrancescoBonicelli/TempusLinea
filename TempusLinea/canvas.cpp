#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    //canvas_start_date = date(1949, 1, 1);
    //canvas_end_date = date(2049, 1, 1);
    canvas_start_date = date(6949, 1, 1);
    canvas_end_date = date(7049, 1, 1);

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
    std::array<int, 3> ticks_width_array{ 1, 2, 5};
    int ticks_width = ticks_width_array[0];
    for (int i = 0; (canvas_end_date.year() - canvas_start_date.year()) / ticks_width > width() / 100; i++)
    {
        ticks_width = ticks_width_array[i % 3] * std::pow(10, int(i / 3));

        // if i >= 10 -> exception when computing years()
        if (i >= 9) break;
    }

    // Draw the timeline ticks
    int x_span = (canvas_end_date-canvas_start_date).days();
    date first_tick_date(canvas_start_date.year() - (canvas_start_date.year() % ticks_width), 1, 1);
    for (date d = first_tick_date; d <= canvas_end_date; d += years(ticks_width))
    {
        int x = width() * (d - canvas_start_date).days() / x_span;
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
        canvas_start_date -= days((event->pos().x() - starting_drag_position.x()) * (canvas_end_date - canvas_start_date).days() / width());
        canvas_end_date -= days((event->pos().x() - starting_drag_position.x()) * (canvas_end_date - canvas_start_date).days() / width());
        starting_drag_position = event->pos();
        update();
    }
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    int delta_x = event->angleDelta().y() * (canvas_end_date - canvas_start_date).days() / width();

    // Reducing range -> min 5 years
    if (delta_x > 0 && canvas_end_date - years(5) < canvas_start_date + days(delta_x))
    {
        delta_x = (canvas_end_date - (canvas_start_date + years(5))).days();
    }
    // Enlarging range -> max 2000 years
    else if (delta_x < 0 && canvas_end_date - years(2000) > canvas_start_date + days(delta_x))
    {
        delta_x = (canvas_end_date - (canvas_start_date + years(2000))).days();
    }

    float cursor_position = (float)event->position().x() / width();
    canvas_start_date += days(delta_x * cursor_position);
    canvas_end_date -= days(delta_x * (1 - cursor_position));
    update();
}