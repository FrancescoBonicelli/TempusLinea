#include "period.h"

Period::Period()
{

}

Period::Period(QString name, QDate starting_date, QDate ending_date, QString& category)
{
    this->name = name;
    this->starting_date = starting_date;
    this->ending_date = ending_date;
    this->category = &category;
}

void Period::setName(QString name)
{
    this->name = name;
}

void Period::setStartingDate(QDate starting_date)
{
    this->starting_date = starting_date;
}

void Period::setEndingDate(QDate ending_date)
{
    this->ending_date = ending_date;
}

void Period::setCategory(QString& category)
{
    this->category = &category;
}

QString Period::getName()
{
    return name;
}

QDate Period::getStartingDate()
{
    return starting_date;
}

QDate Period::getEndingDate()
{
    return ending_date;
}

QString Period::getCategory()
{
    return *category;
}

void Period::read(const QJsonObject& json)
{
    if (json.contains("period") && json["period"].isString())
    {
        QStringList dates = json["period"].toString().split(", ");
        starting_date = QDate::fromString(dates.first(), Qt::ISODate);
        ending_date = QDate::fromString(dates.last(), Qt::ISODate);
    }
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();
}

void Period::write(QJsonObject& json) const
{
    json["period"] = starting_date.toString(Qt::ISODate) + ", " + ending_date.toString(Qt::ISODate);
    json["name"] = name;
}
