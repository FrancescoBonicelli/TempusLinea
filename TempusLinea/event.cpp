#include "event.h"

Event::Event()
{

}

Event::Event(QString name, QDate date)
{
    this->name = name;
    this->date = date;
}

void Event::setName(QString name)
{
    this->name = name;
}

void Event::setDate(QDate date)
{
    this->date = date;
}

QString Event::getName()
{
    return name;
}

QDate Event::getDate()
{
    return date;
}

void Event::read(const QJsonObject& json)
{
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    if (json.contains("date") && json["date"].isString())
        date = QDate::fromString(json["date"].toString());
}

void Event::write(QJsonObject& json) const
{
    json["name"] = name;
    json["date"] = date.toString(Qt::ISODate);
}
