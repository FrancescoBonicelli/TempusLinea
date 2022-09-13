#include "era.h"

Era::Era()
{
}

Era::Era(date starting_date, date ending_date)
{
    era_period = new date_period(starting_date, ending_date);
}

Era::Era(QString name, date starting_date, date ending_date, QColor color)
{
    era_period = new date_period(starting_date, ending_date);
    era_name = name;
    era_color = color;
}

Era::~Era()
{
    //delete era_period;
}

date_period Era::getPeriod()
{
    return *era_period;
}

date Era::getStartingDate()
{
    return era_period->begin();
}

date Era::getEndingDate()
{
    return era_period->end();
}

void Era::setName(QString name)
{
    era_name = name;
}

QString Era::getName()
{
    return era_name;
}

void Era::setColor(QColor color)
{
    era_color = color;
}

QColor Era::getColor()
{
    return era_color;
}

void Era::read(const QJsonObject& json)
{
    if (json.contains("era_period") && json["era_period"].isString())
    {
        QStringList dates = json["era_period"].toString().remove('[').remove(']').split('/');

        era_period = new date_period(
            from_string(dates.first().toStdString()),
            from_string(dates.last().toStdString()));
    }
    if (json.contains("era_name") && json["era_name"].isString())
        era_name = json["era_name"].toString();
    if (json.contains("era_color") && json["era_color"].isString())
        era_color = json["era_color"].toString();
}

void Era::write(QJsonObject& json) const
{
    json["era_period"] = QString::fromStdString(to_simple_string(*era_period));
    json["era_name"] = era_name;
    json["era_color"] = era_color.name(QColor::HexArgb);
}
