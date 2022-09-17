#include "canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    canvas_start_date = QDate(1949, 1, 1);
    canvas_end_date = QDate(2049, 1, 1);

    v_offset = 0;
    dragging = false;

    eras_vector.push_back(new Era("Test_1", QDate(2000, 1, 1), QDate(2100, 1, 1), QColor(255, 0, 0, 50), this));  // Test era 1
    eras_vector.push_back(new Era("Test_2_with_a_really_long_name", QDate(1800, 1, 1), QDate(1900, 1, 1), QColor(0, 255, 0, 50), this));  // Test era 2
    eras_vector.push_back(new Era("Test_2_with_a_really_long_name", QDate(-1, 1, 1), QDate(10, 1, 1), QColor(0, 255, 0, 50), this));  // Test era 2

    // Implement mouse menu
    mouse_menu = new MouseMenu(this);
    mouse_menu->resize(QSize(80, 80));
    mouse_menu->setVisible(false);

    connect(mouse_menu, SIGNAL(newEraClicked()), this, SLOT(openEraCreationDialog()));
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint Eras
    for(Era* e : eras_vector)
    {
        // Paint eras background
        QPoint starting_point(getDatePosition(e->getStartingDate()), 0);
        QPoint ending_point(getDatePosition(e->getEndingDate()), height());

        painter.fillRect(QRect(starting_point, ending_point), e->getColor());

        // Paint era labels (widget)
        // Get label width
        QFontMetrics fm = painter.fontMetrics();
        int label_width = fm.horizontalAdvance(e->getName());

        int i = 0;
        int label_height = 30;
        if (e->getStartingDate() > canvas_start_date)
        {

            e->setVisible(false);

            // Compute vertical position to avoid overlapping
            while (Era* test = dynamic_cast<Era*>(this->childAt(QPoint(getDatePosition(e->getStartingDate()), height() - (0.5 + i) * label_height))))
            {
                i++;
            }

            // Place and show the widget
            e->setGeometry(QRect(getDatePosition(e->getStartingDate()), height() - label_height * (1 + i),
                label_width + 20, label_height));
            e->setVisible(true);
        }
        else if (e->getEndingDate() > canvas_start_date.addDays(0.02 * (canvas_start_date.daysTo(canvas_end_date))))
        {
            // Place at the left margin and show the widget
            e->setGeometry(QRect(0, height() - label_height,
                label_width + 20, label_height));
            e->setVisible(true);
        }
        else e->setVisible(false);
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

    // TODO: fix negative to positive dates transition
    // Draw the timeline ticks
    QDate first_tick_date(canvas_start_date.addYears(- canvas_start_date.year() % ticks_width).year(), 1, 1);
    for (QDate d = first_tick_date; d <= canvas_end_date; d = d.addYears(ticks_width))
    {
        int x = getDatePosition(d);
        painter.drawLine(x, y + 5, x, y - 5);
        painter.drawText(x, y - 10, std::to_string(d.year()).data());
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        starting_drag_position = event->pos();
        dragging = true;
    }
    else dragging = false;

    if (mouse_menu->isVisible())
    {
        mouse_menu->setVisible(false);
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{
        QPoint mouse_position = event->pos();
        mouse_menu->setGeometry(QRect(mouse_position - QPoint(MOUSE_MENU_SIZE/2, MOUSE_MENU_SIZE/2), QSize(MOUSE_MENU_SIZE, MOUSE_MENU_SIZE)));
        mouse_menu->setVisible(true);
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
        canvas_start_date = canvas_start_date.addDays(-(event->pos().x() - starting_drag_position.x()) * (canvas_start_date.daysTo(canvas_end_date)) / width());
        canvas_end_date = canvas_end_date.addDays(-(event->pos().x() - starting_drag_position.x()) * (canvas_start_date.daysTo(canvas_end_date)) / width());
        starting_drag_position = event->pos();
        update();
    }
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    int delta_x = event->angleDelta().y() * (canvas_start_date.daysTo(canvas_end_date)) / width();

    // Reducing range -> min 5 years
    if (delta_x > 0 && canvas_end_date.addYears(-5) < canvas_start_date.addDays(delta_x))
    {
        delta_x = canvas_start_date.addYears(5).daysTo(canvas_end_date);
    }
    // Enlarging range -> max 2000 years
    else if (delta_x < 0 && canvas_end_date.addYears(-2000) > canvas_start_date.addDays(delta_x))
    {
        delta_x = canvas_start_date.addYears(2000).daysTo(canvas_end_date);
    }

    float cursor_position = (float)event->position().x() / width();
    canvas_start_date = canvas_start_date.addDays(delta_x * cursor_position);
    canvas_end_date = canvas_end_date.addDays(-delta_x * (1 - cursor_position));
    update();

    if (mouse_menu->isVisible())
    {
        mouse_menu->setVisible(false);
    }
}

int Canvas::getDatePosition(QDate d)
{
    int x_span = canvas_start_date.daysTo(canvas_end_date);
    return width() * canvas_start_date.daysTo(d) / x_span;
}

void Canvas::read(const QJsonObject& json)
{
    if (json.contains("eras") && json["eras"].isArray()) {
        QJsonArray eras_array = json["eras"].toArray();
        eras_vector.clear();
        for (int era_index = 0; era_index < eras_array.size(); ++era_index) {
            QJsonObject era_obj = eras_array[era_index].toObject();
            Era* era;
            era->read(era_obj);
            eras_vector.push_back(era);
        }
    }
}

void Canvas::write(QJsonObject& json) const
{
    QJsonArray eras_array;
    for (Era* e : eras_vector)
    {
        QJsonObject era_object;
        e->write(era_object);
        eras_array.append(era_object);
    }
    json["eras"] = eras_array;
}

void Canvas::openEraCreationDialog()
{
    EraForm dialog(tr("New Era Details"), this);

    if (dialog.exec() == QDialog::Accepted)
    {
        eras_vector.push_back(new Era(dialog.name(), dialog.startingDate(), dialog.endingDate(), dialog.color(), this));
    }
}
