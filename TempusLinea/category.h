#ifndef CATEGORY_H
#define CATEGORY_H

#include <vector>
#include <QString>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>

#include "event.h"
#include "period.h"

class Category
{
public:
    Category();
    Category(QString name, QColor color);

    void setName(QString name);
    void setColor(QColor color);
    void setVisibility(bool visible);

    QString getName();
    QColor getColor();
    bool isVisible();

    std::vector<Event> events;
    std::vector<Period> periods;

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    QString name;
    QColor color;
    bool visible;
};

#endif // CATEGORY_H
