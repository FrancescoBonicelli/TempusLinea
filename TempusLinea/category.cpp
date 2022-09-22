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
