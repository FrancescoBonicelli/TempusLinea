#ifndef CATEGORY_H
#define CATEGORY_H

#include <vector>
#include <QString>
#include <QColor>

#include "event.h"
#include "period.h"

class Category
{
public:
    Category();
    Category(QString name, QColor color);

    void setName(QString name);
    void setColor(QColor color);

    QString getName();
    QColor getColor();

    std::vector<Event> events;
    std::vector<Period> periods;

private:
    QString name;
    QColor color;
};

#endif // CATEGORY_H
