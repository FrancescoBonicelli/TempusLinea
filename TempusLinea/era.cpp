#include "era.h"

Era::Era(date starting_date, date ending_date)
{
    era_period = new date_period(starting_date, ending_date);
}

Era::~Era()
{
    delete era_period;
}

date_period Era::getPeriod()
{
    return *era_period;
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
