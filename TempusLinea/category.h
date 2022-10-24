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

#include "conf.h"

class CategoryController : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryController(QColor color, bool& visibility, QWidget* parent = nullptr);
    void setColor(QColor color);

private:
    bool* visibility;
    QColor color;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

};

class Category
{
public:
    Category(QWidget* canvas);
    Category(QString name, QColor color, QWidget* canvas);
    ~Category();

    QString name;

    void setName(QString name);
    void setColor(QColor color);
    void setVisibility(bool visible);

    QString getName();
    QColor getColor();
    bool isVisible();
    bool isCollapsed();
    bool isDefault();

    void computeBoundingRect();
    QRect getBoundingRect();

    std::vector<Event*> events;
    std::vector<Period*> periods;
    std::vector<Era*> eras;

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    CategoryController* control;

private:
    QColor color;
    bool visible;
    bool collapsed;
    QWidget* canvas;
    QRect bounding_rect;
};

#endif // CATEGORY_H
