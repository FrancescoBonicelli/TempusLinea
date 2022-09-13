#ifndef ERA_H
#define ERA_H

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <QPainter>
#include <QJsonObject>

using namespace boost::gregorian;

class Era
{
public:
    Era();
    Era(date starting_date, date ending_date);
    Era(QString, date starting_date, date ending_date, QColor color);
    ~Era();

    date_period getPeriod();
    date getStartingDate();
    date getEndingDate();

    void setName(QString name);
    QString getName();

    void setColor(QColor color);
    QColor getColor();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    date_period* era_period;
    QString era_name;
    QColor era_color;
};

#endif // ERA_H
