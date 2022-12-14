#include "category.h"

CategoryController::CategoryController(QColor color, bool& visibility, QWidget* parent) : QWidget{ parent }
{
    this->visibility = &visibility;
    this->color = color;
}

void CategoryController::setColor(QColor color)
{
    this->color = color;
}

void CategoryController::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(width() / 2, height() / 2, width(), height());
    gradient.setColorAt(0, Qt::transparent);
    gradient.setColorAt(1, color);

    QBrush brush(gradient);

    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, width(), height()), brush);
}

void CategoryController::mouseDoubleClickEvent(QMouseEvent *event)
{
    *visibility = !*visibility;
    parentWidget()->update();
}
//----------------------------------------------------------------------------------

Category::Category(QWidget* canvas)
{
    this->canvas = canvas;
    this->visible = true;
    this->collapsed = false;

    bounding_rect = QRect(0, 0, 0, 0);

    control = new CategoryController(color, collapsed, canvas);
}

Category::Category(QString name, QColor color, QWidget* canvas) : Category(canvas)
{
    setName(name);
    setColor(color);
}

Category::~Category()
{
    for (Event* e : events) delete(e);
    for (Period* p : periods) delete(p);
    for (Era* e : eras) delete(e);

    delete control;
}

void Category::setName(QString name)
{
    this->name = name;
}

void Category::setColor(QColor color)
{
    this->color = color;
    control->setColor(color);
}

void Category::setVisibility(bool visible)
{
    this->visible = visible;
    this->canvas->update();
}

QString Category::getName()
{
    return name;
}

QColor Category::getColor()
{
    return color;
}

bool Category::isVisible()
{
    return visible;
}

bool Category::isCollapsed()
{
    return collapsed;
}

bool Category::isDefault()
{
    return name.size() == 0;
}

void Category::read(const QJsonObject& json)
{
    if (json.contains("name") && json["name"].isString())
        setName(json["name"].toString());
    if (json.contains("color") && json["color"].isString())
        setColor(json["color"].toString());
    if (json.contains("visible") && json["visible"].isBool())
        setVisibility(json["visible"].toBool());

    for (Event* e : events)
        delete e;
    events.clear();

    if (json.contains("events") && json["events"].isArray())
    {
        QJsonArray events_array = json["events"].toArray();

        for (int event_index = 0; event_index < events_array.size(); ++event_index)
        {
            QJsonObject event_obj = events_array[event_index].toObject();
            Event* event = new Event(canvas);
            event->read(event_obj);
            event->setCategory(name);
            events.push_back(event);
        }

        sort(events.begin(), events.end(),
            [](Event* a, Event* b) -> bool
            {
                return a->getDate() < b->getDate();
            });
    }

    for (Period* p : periods)
        delete p;
    periods.clear();

    if (json.contains("periods") && json["periods"].isArray())
    {
        QJsonArray periods_array = json["periods"].toArray();

        for (int period_index = 0; period_index < periods_array.size(); ++period_index)
        {
            QJsonObject period_obj = periods_array[period_index].toObject();
            Period* period = new Period(canvas);
            period->read(period_obj);
            period->setCategory(name);
            periods.push_back(period);
        }

        sort(periods.begin(), periods.end(),
            [](Period* a, Period* b) -> bool
            {
                return a->getStartingDate() < b->getStartingDate();
            });
    }

    for (Era* e : eras)
        delete e;
    eras.clear();

    if (json.contains("eras") && json["eras"].isArray())
    {
        QJsonArray eras_array = json["eras"].toArray();

        for (int era_index = 0; era_index < eras_array.size(); ++era_index)
        {
            QJsonObject era_obj = eras_array[era_index].toObject();
            Era* era = new Era(canvas);
            era->read(era_obj);
            era->setCategory(name);
            eras.push_back(era);
        }

        sort(eras.begin(), eras.end(),
            [](Era* a, Era* b) -> bool
            {
                return a->getStartingDate() < b->getStartingDate();
            });
    }
}

void Category::write(QJsonObject& json) const
{
    json["name"] = name;
    json["color"] = color.name(QColor::HexArgb);
    json["visible"] = visible;

    QJsonArray events_array;
    for (Event* e : events)
    {
        QJsonObject event_object;
        e->write(event_object);
        events_array.append(event_object);
    }
    json["events"] = events_array;

    QJsonArray periods_array;
    for (Period* p : periods)
    {
        QJsonObject period_object;
        p->write(period_object);
        periods_array.append(period_object);
    }
    json["periods"] = periods_array;

    QJsonArray eras_array;
    for (Era* e : eras)
    {
        QJsonObject era_object;
        e->write(era_object);
        eras_array.append(era_object);
    }
    json["eras"] = eras_array;
}

void Category::computeBoundingRect()
{
    if(periods.size() > 0)
    {
        bounding_rect = periods.at(0)->period_rect;

        for(Period* p : periods)
        {
            int x1, y1, x2, y2;
            p->period_rect.getCoords(&x1, &y1, &x2, &y2);

            // Place clearance
            x1 -= 10;
            y1 -= 10;
            x2 += 10;

            int xx1, yy1, xx2, yy2;
            bounding_rect.getCoords(&xx1, &yy1, &xx2, &yy2);

            int final_x1 = x1 < xx1 ? x1 : xx1;
            int final_y1 = y1 < yy1 ? y1 : yy1;
            int final_x2 = x2 > xx2 ? x2 : xx2;
            int final_y2 = y2 > yy2 ? y2 : yy2;

            bounding_rect = QRect(QPoint(final_x1, final_y1), QPoint(final_x2, final_y2));
        }

        if (!isDefault()) bounding_rect.adjust(0, 0, 0, category_controller_size);
    }
    else
    {
        bounding_rect = QRect(0, 0, 0, 0);
    }
}

QRect Category::getBoundingRect()
{
    return bounding_rect;
}
