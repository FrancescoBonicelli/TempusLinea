#include "canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    //canvas_start_date = date(1949, 1, 1);
    //canvas_end_date = date(2049, 1, 1);
    canvas_start_date = date(6949, 1, 1);
    canvas_end_date = date(7049, 1, 1);

    v_offset = 0;
    dragging = false;

    eras_vector.push_back(Era("Test_1", date(7000, 1, 1), date(7100, 1, 1), QColor(255, 0, 0, 50)));  // Test era 1
    eras_vector.push_back(Era("Test_2", date(6800, 1, 1), date(6900, 1, 1), QColor(0, 255, 0, 50)));  // Test era 2

    // Implement mouse menu
    mouse_menu = new MouseMenu(this);
    mouse_menu->resize(QSize(80, 80));
    mouse_menu->setVisible(false);
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint Eras
    for(Era e : eras_vector)
    {
        QPoint starting_point(getDatePosition(e.getStartingDate()), 0);
        QPoint ending_point(getDatePosition(e.getEndingDate()), height());

        painter.fillRect(QRect(starting_point, ending_point), e.getColor());
    }

    // Paint Timeline
    QColor timeline_color(0, 0, 0);
    painter.setPen(timeline_color);
    int y = (height() / 2) + v_offset;
    painter.drawLine(0, y, width(), y);

    // Compute the timelline ticks
    std::array<int, 3> ticks_width_array{ 1, 2, 5};
    int ticks_width = ticks_width_array[0];
    for (int i = 0; (canvas_end_date.year() - canvas_start_date.year()) / ticks_width > width() / 100; i++)
    {
        ticks_width = ticks_width_array[i % ticks_width_array.size()] * std::pow(10, int(i / ticks_width_array.size()));

        // if i >= 10 -> exception when computing years()
        if (i >= 3 * ticks_width_array.size()) break;
    }

    // Draw the timeline ticks
    date first_tick_date(canvas_start_date.year() - (canvas_start_date.year() % ticks_width), 1, 1);
    for (date d = first_tick_date; d <= canvas_end_date; d += years(ticks_width))
    {
        int x = getDatePosition(d);
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

    if (event->button() == Qt::LeftButton)
    {
        QPoint mouse_position = event->pos();
        mouse_menu->setGeometry(QRect(mouse_position - QPoint(MOUSE_MENU_SIZE/2, MOUSE_MENU_SIZE/2), QSize(MOUSE_MENU_SIZE, MOUSE_MENU_SIZE)));
        mouse_menu->setVisible(true);
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
    // TODO: prevent dates < 1400

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
    // TODO: prevent dates < 1400

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

int Canvas::getDatePosition(date d)
{
    int x_span = (canvas_end_date - canvas_start_date).days();
    return width() * (d - canvas_start_date).days() / x_span;
}

void Canvas::read(const QJsonObject& json)
{
    if (json.contains("eras") && json["eras"].isArray()) {
        QJsonArray eras_array = json["eras"].toArray();
        eras_vector.clear();
        for (int era_index = 0; era_index < eras_array.size(); ++era_index) {
            QJsonObject era_obj = eras_array[era_index].toObject();
            Era era;
            era.read(era_obj);
            eras_vector.push_back(era);
        }
    }
}

void Canvas::write(QJsonObject& json) const
{
    QJsonArray eras_array;
    for (Era e : eras_vector)
    {
        QJsonObject era_object;
        e.write(era_object);
        eras_array.append(era_object);
    }
    json["eras"] = eras_array;
}
