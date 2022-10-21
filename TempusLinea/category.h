#ifndef CATEGORY_H
#define CATEGORY_H

#include <vector>
#include <QString>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QWidget>

#include "event.h"
#include "period.h"
#include "era.h"

class Category
{
public:
    Category(QWidget* canvas);
    Category(QString name, QColor color, QWidget* canvas);

    QString name;

    void setName(QString name);
    void setColor(QColor color);
    void setVisibility(bool visible);

    QString getName();
    QColor getColor();
    bool isVisible();
    bool isCollapsed();

    void computeBoundingRect();
    QRect getBoundingRect();

    std::vector<Event*> events;
    std::vector<Period*> periods;
    std::vector<Era*> eras;

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    QColor color;
    bool visible;
    bool collapsed;
    QWidget* canvas;
    QRect bounding_rect;
};

#endif // CATEGORY_H
