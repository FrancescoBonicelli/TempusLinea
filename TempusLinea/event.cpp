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
