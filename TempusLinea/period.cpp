#include "period.h"

Period::Period()
{

}

Period::Period(QString name, QDate starting_date, QDate ending_date)
{
    this->name = name;
    this->starting_date = starting_date;
    this->ending_date = ending_date;
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
