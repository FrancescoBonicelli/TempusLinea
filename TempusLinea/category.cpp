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

QString Category::getName()
{
    return name;
}

QColor Category::getColor()
{
    return color;
}
