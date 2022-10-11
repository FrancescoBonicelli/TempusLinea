#include "canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget{parent}
{
    //Setup default parameters
    canvas_start_date = QDate(QDate::currentDate().year() - 80, 1, 1);
    canvas_end_date = QDate(QDate::currentDate().year() + 20, 1, 1);

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
    categories.push_back(new Category("", Qt::black, this));
    // Implement Categories Manager
    categories_manager = new CategoriesManager(categories, this);
    categories_manager->move(QPoint(width() - categories_manager->width(), 0) + CATEGORIES_MANAGER_MARGINS);
    categories_manager->resize(CATEGORIES_MANAGER_WIDTH, 35 + 20*(categories.size()-1));

    connect(categories_manager, &CategoriesManager::resized, [this](){categories_manager->move(QPoint(width() - categories_manager->width(), 0) + CATEGORIES_MANAGER_MARGINS);});
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QFontMetrics fm = painter.fontMetrics();

    // Timeline axis y position
    int timeline_y = (height() / 2) + v_offset;

    // Paint Eras
    for(Era* e : eras_vector)
    {
        // Paint eras background
        QPoint starting_point(getDatePosition(e->getStartingDate()), 0);
        QPoint ending_point(getDatePosition(e->getEndingDate()), height());
        QColor era_color = e->getColor();
        era_color.setAlpha(100);
        QBrush era_brush = QBrush(era_color, Qt::DiagCrossPattern);
        painter.fillRect(QRect(starting_point, ending_point), era_brush);

        // Paint era labels (widget)
        // Get label width
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

    // Paint Events
    std::vector<Event*> events_to_show;
    for(Category* c : categories)
    {
        if (c->isVisible())
        {
            for (Event* e : c->events)
            {
                if (e->getDate() > canvas_start_date && e->getDate() < canvas_end_date)
                {
                    events_to_show.push_back(e);
                }
            }
        }
    }

    placeEvents(events_to_show, fm);

    for (Category* c : categories)
    {
        painter.setPen(QPen(c->getColor()));

        for (Event* e : c->events)
        {
            if (e->getDate() > canvas_start_date && e->getDate() < canvas_end_date)
            {
                e->setGeometry(e->label_rect);
                e->setVisible(c->isVisible());

                if (c->isVisible())
                {
                    // Paint the lines
                    painter.drawLine(e->label_rect.bottomLeft(), e->label_rect.bottomRight());
                    painter.drawLine(getDatePosition(e->getDate()), timeline_y, getDatePosition(e->getDate()), e->label_rect.bottom());
                }
            }
            else e->setVisible(false);
        }
    }

    // Paint Periods
    int category_offset = v_offset;

    for(Category* c : categories)
    {
        // Place and Draw periods for visible categories
        if(c->isVisible())
        {
            placePeriods(c->periods, fm, category_offset);

            painter.setPen(QPen(c->getColor()));

            // Draw Bounding Box
            c->computeBoundingRect();
            category_offset = category_offset + c->getBoundingRect().height() + categories_spacing;
            painter.drawRect(c->getBoundingRect());
        }

        if(!c->isCollapsed())
        {
            // Draw single periods
            for(Period* p : c->periods)
            {
                if (p->getStartingDate() < canvas_end_date && p->getEndingDate() > canvas_start_date)
                {
                    p->setGeometry(p->label_rect);
                    p->setVisible(c->isVisible());

                    if (c->isVisible())
                    {
                        painter.drawLine(p->period_rect.topLeft(), p->period_rect.topRight());
                        painter.drawLine(p->period_rect.topLeft().x(), p->period_rect.topLeft().y() - 4,
                            p->period_rect.topLeft().x(), p->period_rect.topLeft().y() + 4);
                        painter.drawLine(p->period_rect.topRight().x(), p->period_rect.topRight().y() - 4,
                            p->period_rect.topRight().x(), p->period_rect.topRight().y() + 4);
                    }
                }
                else p->setVisible(false);
            }
        }
        else
        {

        }
    }


    // Paint Timeline
    painter.setPen(QPen(Qt::black));
    painter.drawLine(0, timeline_y, width(), timeline_y);

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
        painter.drawLine(x, timeline_y + 5, x, timeline_y - 5);
        painter.drawText(x, timeline_y - 10, std::to_string(d.year()).data());
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
        mouse_menu->setGeometry(QRect(mouse_position - QPoint(mouse_menu_size/2, mouse_menu_size/2), QSize(mouse_menu_size, mouse_menu_size)));
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

QDate Canvas::getDateFromPosition(int p)
{
    int x_span = canvas_start_date.daysTo(canvas_end_date);
    return canvas_start_date.addDays(p / width() * x_span);
}

void Canvas::read(const QJsonObject& json)
{
    resetCanvas();

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
        for (int category_index = 0; category_index < categories_array.size(); ++category_index) {
            QJsonObject category_obj = categories_array[category_index].toObject();
            Category* category = new Category(this);
            category->read(category_obj);
            for (Event* e : category->events)
            {
                connect(e, &Event::editEvent, this, &Canvas::openEventEditDialog);
            }
            for (Period* p : category->periods)
            {
                connect(p, &Period::editPeriod, this, &Canvas::openPeriodEditDialog);
            }
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

void Canvas::resetCanvas()
{
    //Setup default parameters
    canvas_start_date = QDate(QDate::currentDate().year() - 80, 1, 1);
    canvas_end_date = QDate(QDate::currentDate().year() + 20, 1, 1);

    v_offset = 0;
    dragging = false;

    //Clean vectors
    for (Era* e : eras_vector) delete(e);
    eras_vector.clear();

    for (Category* c : categories)
    {
        for (Event* e : c->events) delete(e);
        c->events.clear();
        for (Period* p : c->periods) delete(p);
        c->periods.clear();
        delete(c);
    }
    categories.clear();
    categories_manager->refreshCategories();
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
        Event* new_event = new Event(dialog.name(), dialog.date(), dialog.category()->name, this);
        connect(new_event, &Event::editEvent, this, &Canvas::openEventEditDialog);
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
    EventForm dialog(event->getName() + tr(" Event Details"), event, categories, this);

    int out = dialog.exec();
    if (out == QDialog::Accepted)
    {
        // Remove the event from the old category
        for (Category* c : categories)
        {
            for (Event* e : c->events)
            {
                if(e == event) c->events.erase(std::remove(c->events.begin(), c->events.end(), e), c->events.end());
            }
        }

        // Edit the event
        event->setName(dialog.name());
        event->setDate(dialog.date());
        event->setCategory(dialog.category()->name);

        // Insert the event in the new category
        for (Category* c : categories)
        {
            if (c == dialog.category())
            {
                c->events.push_back(event);
            }
        }
    }
    else if (out == QDialog::Rejected && dialog.name() == "~")
    {
        delete(event);
        for (Category* c : categories)
        {
            c->events.erase(std::remove(c->events.begin(), c->events.end(), event), c->events.end());
        }
    }
}

void Canvas::openPeriodCreationDialog()
{
    PeriodForm dialog(tr("New Period Details"), categories, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Period* new_period = new Period(dialog.name(), dialog.starting_date(), dialog.ending_date(), dialog.category()->name, this);
        connect(new_period, &Period::editPeriod, this, &Canvas::openPeriodEditDialog);
        for (Category* c : categories)
        {
            if (c == dialog.category())
            {
                c->periods.push_back(new_period);
            }
        }
    }
}

void Canvas::openPeriodEditDialog(Period* period)
{
    PeriodForm dialog(period->getName() + tr(" Period Details"), period, categories, this);

    int out = dialog.exec();
    if (out == QDialog::Accepted)
    {
        // Remove the event from the old category
        for (Category* c : categories)
        {
            for (Period* p : c->periods)
            {
                if (p == period) c->periods.erase(std::remove(c->periods.begin(), c->periods.end(), p), c->periods.end());
            }
        }

        // Edit the event
        period->setName(dialog.name());
        period->setStartingDate(dialog.starting_date());
        period->setEndingDate(dialog.ending_date());
        period->setCategory(dialog.category()->name);

        // Insert the event in the new category
        for (Category* c : categories)
        {
            if (c == dialog.category())
            {
                c->periods.push_back(period);
            }
        }
    }
    else if (out == QDialog::Rejected && dialog.name() == "~")
    {
        delete(period);
        for (Category* c : categories)
        {
            c->periods.erase(std::remove(c->periods.begin(), c->periods.end(), period), c->periods.end());
        }
    }
}

void Canvas::placeEvents(std::vector<Event*> events_vector, QFontMetrics fm)
{
    int event_height = event_label_height;
    int event_start_y = (height() / 2) + v_offset - 60;

    for(Event* e : events_vector)
    {
        int event_width = fm.horizontalAdvance(e->getName()) + 2;
        int event_start_x = getDatePosition(e->getDate())-event_width/2;

        e->label_rect = QRect(event_start_x, event_start_y, event_width, event_height);
    }

    for(int i = 0; i < events_vector.size(); i++)
    {
        bool placed = false;

        while(!placed)
        {
            placed = true;

            for (int y = 0; y < i; y++)
            {
                if (events_vector.at(i)->label_rect.adjusted(1, 1, -1, -1).intersects(events_vector.at(y)->label_rect))
                {
                    placed = false;
                    events_vector.at(i)->label_rect.translate(0, -1.1 * event_label_height);
                    break;
                }
            }
        }
    }
}

void Canvas::placePeriods(std::vector<Period*> periods_vector_full, QFontMetrics fm, int v_offset)
{
    std::vector<Period*> periods_vector;
    for (Period* p : periods_vector_full)
    {
        if (p->getStartingDate() < canvas_end_date && p->getEndingDate() > canvas_start_date)
        {
            periods_vector.push_back(p);
        }
    }
    int period_height = period_label_height;
    int period_start_y = (height() / 2) + v_offset + 20;

    for(Period* p : periods_vector)
    {
        int period_start_x = getDatePosition(p->getStartingDate());
        int period_end_x = getDatePosition(p->getEndingDate());

        int label_start_x = 0;
        int label_width = fm.horizontalAdvance(p->getName()) + 2;

        if(period_end_x - period_start_x < label_width)  // If label wider than period
        {
            label_start_x = period_start_x;  // Place label start at period's start point
        }
        else
        {
            label_start_x = p->getStartingDate() > canvas_start_date ? period_start_x : 0;  // Follow left margin

            if(label_start_x + label_width > period_end_x)  // If label overcome period's right limit
            {
                label_start_x = period_end_x - label_width;
            }

            // Move the label to the right to avoid period start bar
            label_start_x += 2;
        }

        p->label_rect = QRect(label_start_x, period_start_y, label_width, period_height);
        p->period_rect = QRect(QPoint(period_start_x, period_start_y), QPoint(period_end_x, period_start_y+period_height));
    }

    for(int i = 0; i < periods_vector.size(); i++)
    {
        bool placed = false;

        while(!placed)
        {
            placed = true;

            for (int y = 0; y < i; y++)
            {
                if(   periods_vector.at(i)->label_rect.adjusted(1,1,-1,-1).intersects(periods_vector.at(y)->label_rect)
                   || periods_vector.at(i)->label_rect.adjusted(1, 1, -1, -1).intersects(periods_vector.at(y)->period_rect)
                   || periods_vector.at(i)->period_rect.adjusted(1,1,-1,-1).intersects(periods_vector.at(y)->label_rect)
                   || periods_vector.at(i)->period_rect.adjusted(1,1,-1,-1).intersects(periods_vector.at(y)->period_rect))
                {
                    placed = false;
                    periods_vector.at(i)->label_rect.translate(0, 1.1 * period_label_height);
                    periods_vector.at(i)->period_rect.translate(0, 1.1 * period_label_height);
                    break;
                }
            }
        }
    }
}
