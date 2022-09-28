#include "event.h"

Event::Event()
{

}

Event::Event(QString name, QDate date, QString& category)
{
    this->name = name;
    this->date = date;
    this->category = &category;
}

void Event::setName(QString name)
{
    this->name = name;
}

void Event::setDate(QDate date)
{
    this->date = date;
}

void Event::setCategory(QString& category)
{
    this->category = &category;
}

QString Event::getName()
{
    return name;
}

QDate Event::getDate()
{
    return date;
}

QString Event::getCategory()
{
    return *category;
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
