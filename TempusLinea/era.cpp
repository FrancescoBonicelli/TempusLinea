#include "era.h"

Era::Era(date starting_date, date ending_date)
{
    era_period = new date_period(starting_date, ending_date);
}

Era::Era(std::string name, date starting_date, date ending_date, QColor color)
{
    era_period = new date_period(starting_date, ending_date);
    era_name = name;
    era_color = color;
}

Era::~Era()
{
    delete era_period;
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

void Era::setName(std::string name)
{
    era_name = name;
}

std::string Era::getName()
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
