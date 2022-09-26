#include "category.h"

Category::Category()
{

}

Category::Category(QString name, QColor color)
{
    this->name = name;
    this->color = color;
}

void Category::setName(QString name)
{
    this->name = name;
}

void Category::setColor(QColor color)
{
    this->color = color;
}

void Category::setVisibility(bool visible)
{
    this->visible = visible;
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

void Category::read(const QJsonObject& json)
{
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    if (json.contains("color") && json["color"].isString())
        color = json["color"].toString();
    if (json.contains("visible") && json["visible"].isBool())
        visible = json["visible"].toBool();

    if (json.contains("events") && json["events"].isArray()) {
        QJsonArray events_array = json["events"].toArray();
        events.clear();
        for (int event_index = 0; event_index < events_array.size(); ++event_index) {
            QJsonObject event_obj = events_array[event_index].toObject();
            Event event = Event();
            event.read(event_obj);
            events.push_back(event);
        }
    }

    if (json.contains("periods") && json["periods"].isArray()) {
        QJsonArray periods_array = json["periods"].toArray();
        periods.clear();
        for (int period_index = 0; period_index < periods_array.size(); ++period_index) {
            QJsonObject period_obj = periods_array[period_index].toObject();
            Period period = Period();
            period.read(period_obj);
            periods.push_back(period);
        }
    }
}

void Category::write(QJsonObject& json) const
{
    json["name"] = name;
    json["color"] = color.name(QColor::HexArgb);
    json["visible"] = visible;

    QJsonArray events_array;
    for (Event e : events)
    {
        QJsonObject event_object;
        e.write(event_object);
        events_array.append(event_object);
    }
    json["events"] = events_array;

    QJsonArray periods_array;
    for (Period p : periods)
    {
        QJsonObject period_object;
        p.write(period_object);
        periods_array.append(period_object);
    }
    json["periods"] = periods_array;
}
