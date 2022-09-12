#ifndef ERA_H
#define ERA_H

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <QPainter>

using namespace boost::gregorian;

class Era
{
public:
    Era(date starting_date, date ending_date);
    ~Era();

    date_period getPeriod();

    void setName(std::string name);
    std::string getName();

    void setColor(QColor color);
    QColor getColor();

private:
    date_period* era_period;
    std::string era_name;
    QColor era_color;
};

#endif // ERA_H
