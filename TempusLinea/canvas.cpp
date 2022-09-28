#include "canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    canvas_start_date = QDate(1929, 1, 1);
    canvas_end_date = QDate(2029, 1, 1);

    v_offset = 0;
    dragging = false;

    // Implement mouse menu
    mouse_menu = new MouseMenu(this);
    mouse_menu->resize(QSize(80, 80));
    mouse_menu->setVisible(false);

    connect(mouse_menu, &MouseMenu::newEraClicked, this, &Canvas::openEraCreationDialog);
    connect(mouse_menu, &MouseMenu::newEventClicked, this, &Canvas::openEventCreationDialog);
    connect(mouse_menu, &MouseMenu::newPeriodClicked, this, &Canvas::openPeriodCreationDialog);

    // Add default category
    categories.push_back(new Category("", Qt::transparent));
    // Implement Categories Manager
    categories_manager = new CategoriesManager(categories, this);
    categories_manager->move(QPoint(width() - categories_manager->width(), 0) + CATEGORIES_MANAGER_MARGINS);
    categories_manager->resize(CATEGORIES_MANAGER_WIDTH, 35 + 20*(categories.size()-1));

    connect(categories_manager, &CategoriesManager::resized, [this](){categories_manager->move(QPoint(width() - categories_manager->width(), 0) + CATEGORIES_MANAGER_MARGINS);});

    // ---------- TEST LINES - TO BE REMOVED ----------

    eras_vector.push_back(new Era("Test_1", QDate(2000, 1, 1), QDate(2100, 1, 1), QColor(255, 0, 0), this));  // Test era 1
    eras_vector.push_back(new Era("Test_2_with_a_really_long_name", QDate(1800, 1, 1), QDate(1900, 1, 1), QColor(0, 255, 0), this));  // Test era 2

    for(int i = 0; i < eras_vector.size(); i++)
    {
        connect(eras_vector.at(i), &Era::editEra, this, &Canvas::openEraEditDialog);
    }

    // ---------- END OF TEST LINES ----------
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Paint Eras
    for(Era* e : eras_vector)
    {
        // Paint eras background
        QPoint starting_point(getDatePosition(e->getStartingDate()), 0);
        QPoint ending_point(getDatePosition(e->getEndingDate()), height());
        QColor era_color = e->getColor();
        era_color.setAlpha(50);
        painter.fillRect(QRect(starting_point, ending_point), era_color);

        // Paint era labels (widget)
        // Get label width
        QFontMetrics fm = painter.fontMetrics();
        int label_width = fm.horizontalAdvance(e->getName()) + 20;
        int label_height = 30;

        int i = 0;
        // Compute vertical position to avoid overlapping
        while (Era* test = dynamic_cast<Era*>(this->childAt(QPoint(getDatePosition(e->getStartingDate()), height() - (0.5 + i) * label_height))))
        {
            if (test == e) break;
            i++;
        }

        if (e->getStartingDate() > canvas_start_date)
        {
            // Place and show the widget
            e->setGeometry(QRect(getDatePosition(e->getStartingDate()), height() - label_height * (1 + i),
                label_width, label_height));
            e->setVisible(true);
        }
        else if (e->getEndingDate() > canvas_start_date)
        {
            // If the visible era is larger than the label width, place the label to the left margin
            if (getDatePosition(e->getEndingDate()) > label_width)
            {
                e->setGeometry(QRect(0, height() - label_height * (1 + i), label_width, label_height));
            }
            // If the era is larger than the label width, place the end of the label @ era end date
            else if (getDatePosition(e->getEndingDate()) - getDatePosition(e->getStartingDate()) > label_width)
            {
                e->setGeometry(QRect(getDatePosition(e->getEndingDate()) - label_width, height() - label_height * (1 + i),
                    label_width, label_height));
            }
            // Else place the start of the label @ era start date
            else
            {
                e->setGeometry(QRect(getDatePosition(e->getStartingDate()), height() - label_height * (1 + i),
                    label_width, label_height));
            }
            
            e->setVisible(true);
        }
        else e->setVisible(false);
    }

    // Paint Timeline
    painter.setPen(QPen(Qt::black));
    int y = (height() / 2) + v_offset;
    painter.drawLine(0, y, width(), y);

    // Compute the timelline ticks
    std::array<int, 3> ticks_width_array{ 1, 2, 5};
    int ticks_width = ticks_width_array[0];
    for (int i = 0; (canvas_end_date.year() - canvas_start_date.year()) / ticks_width > width() / 100; i++)
    {
        ticks_width = ticks_width_array[i % ticks_width_array.size()] * std::pow(10, int(i / ticks_width_array.size()));
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

    painter.setPen(QPen(Qt::red, 0.5));
    int current_date_position = getDatePosition(QDate::currentDate());
    painter.drawLine(current_date_position, 0, current_date_position, height());
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    emit mousePress();

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
    emit mouseWheel();

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

void Canvas::resizeEvent(QResizeEvent *event)
{
    categories_manager->move(QPoint(width() - categories_manager->width(), 0) + CATEGORIES_MANAGER_MARGINS);
}

int Canvas::getDatePosition(QDate d)
{
    int x_span = canvas_start_date.daysTo(canvas_end_date);
    return width() * canvas_start_date.daysTo(d) / x_span;
}

void Canvas::read(const QJsonObject& json)
{
    if (json.contains("canvas_period") && json["canvas_period"].isString())
    {
        QStringList dates = json["canvas_period"].toString().split(", ");
        canvas_start_date = QDate::fromString(dates.first(), Qt::ISODate);
        canvas_end_date = QDate::fromString(dates.last(), Qt::ISODate);
    }

    if (json.contains("canvas_v_offset") && json["canvas_v_offset"].isString())
    {
        v_offset = json["canvas_v_offset"].toString().toInt();
    }

    if (json.contains("eras") && json["eras"].isArray()) {
        QJsonArray eras_array = json["eras"].toArray();
        for (Era* e : eras_vector) delete(e);
        eras_vector.clear();
        for (int era_index = 0; era_index < eras_array.size(); ++era_index) {
            QJsonObject era_obj = eras_array[era_index].toObject();
            Era* era = new Era(this);
            era->read(era_obj);
            connect(era, &Era::editEra, this, &Canvas::openEraEditDialog);
            eras_vector.push_back(era);
        }
    }

    if (json.contains("categories") && json["categories"].isArray()) {
        QJsonArray categories_array = json["categories"].toArray();
        for (Category* c : categories) delete(c);
        categories.clear();
        for (int category_index = 0; category_index < categories_array.size(); ++category_index) {
            QJsonObject category_obj = categories_array[category_index].toObject();
            Category* category = new Category();
            category->read(category_obj);
            categories.push_back(category);
        }
        categories_manager->refreshCategories();
    }
}

void Canvas::write(QJsonObject& json) const
{
    json["canvas_period"] = canvas_start_date.toString(Qt::ISODate) + ", " + canvas_end_date.toString(Qt::ISODate);
    json["canvas_v_offset"] = QString::number(v_offset);

    QJsonArray eras_array;
    for (Era* e : eras_vector)
    {
        QJsonObject era_object;
        e->write(era_object);
        eras_array.append(era_object);
    }
    json["eras"] = eras_array;

    QJsonArray categories_array;
    for (Category* c : categories)
    {
        QJsonObject category_object;
        c->write(category_object);
        categories_array.append(category_object);
    }
    json["categories"] = categories_array;
}

void Canvas::openEraCreationDialog()
{
    EraForm dialog(tr("New Era Details"), this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Era* new_era = new Era(dialog.name(), dialog.startingDate(), dialog.endingDate(), dialog.color(), this);
        connect(new_era, &Era::editEra, this, &Canvas::openEraEditDialog);
        eras_vector.push_back(new_era);
    }
}

void Canvas::openEraEditDialog(Era* era)
{
    EraForm dialog(era->getName() + tr(" Era Details"), era, this);

    int out = dialog.exec();
    if (out == QDialog::Accepted)
    {
        era->setName(dialog.name());
        era->setStartingDate(dialog.startingDate());
        era->setEndingDate(dialog.endingDate());
        era->setColor(dialog.color());
    }
    else if (out == QDialog::Rejected && dialog.name() == "~")
    {
        delete(era);
        eras_vector.erase(std::remove(eras_vector.begin(), eras_vector.end(), era), eras_vector.end());
    }
}

void Canvas::openEventCreationDialog()
{
    EventForm dialog(tr("New Event Details"), categories, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Event new_event = Event(dialog.name(), dialog.date(), dialog.category()->name);
        for (Category* c : categories)
        {
            if (c == dialog.category())
            {
                c->events.push_back(new_event);
            }
        }
    }
}

void Canvas::openEventEditDialog(Event* event)
{

}

void Canvas::openPeriodCreationDialog()
{

}

void Canvas::openPeriodEditDialog(Period* period)
{

}
