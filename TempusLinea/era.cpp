#include "era.h"

Era::Era(QWidget* parent)
{
    setVisible(false);
}

Era::Era(const Era& era) : QWidget{ era.era_parent }
{
    era_name = era.era_name;
    era_color = era.era_color;
    era_starting_date = era.era_starting_date;
    era_ending_date = era.era_ending_date;
    setVisible(false);
}

Era::Era(QString name, QDate starting_date, QDate ending_date, QColor color, QWidget* parent) : QWidget{ parent }
{
    era_name = name;
    era_color = color;
    era_starting_date = starting_date;
    era_ending_date = ending_date;

    era_parent = parent;
    setVisible(false);
}

Era::~Era()
{
    //delete era_period;
}

QDate Era::getStartingDate()
{
    return era_starting_date;
}

void Era::setStartingDate(QDate starting_date)
{
    era_starting_date = starting_date;
}

QDate Era::getEndingDate()
{
    return era_ending_date;
}

void Era::setEndingDate(QDate ending_date)
{
    era_ending_date = ending_date;
}

void Era::setName(QString name)
{
    era_name = name;
}

QString Era::getName()
{
    return era_name;
}

void Era::setColor(QColor color)
{
    era_color = color;
}

QColor Era::getColor()
{
    return era_color;
}

void Era::read(const QJsonObject& json)
{
    if (json.contains("era_period") && json["era_period"].isString())
    {
        QStringList era_dates = json["era_period"].toString().split(", ");
        era_starting_date = QDate::fromString(era_dates.first());
        era_ending_date = QDate::fromString(era_dates.last());
    }
    if (json.contains("era_name") && json["era_name"].isString())
        era_name = json["era_name"].toString();
    if (json.contains("era_color") && json["era_color"].isString())
        era_color = json["era_color"].toString();
}

void Era::write(QJsonObject& json) const
{
    json["era_period"] = era_starting_date.toString() + ", " + era_ending_date.toString();
    json["era_name"] = era_name;
    json["era_color"] = era_color.name(QColor::HexArgb);
}

void Era::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPoint starting_point(5, 5);
    QPoint ending_point(width()-5, height()-5);

    QRect rect = QRect(starting_point, ending_point);

    painter.drawRoundedRect(rect, 5, 5);
    painter.drawText(rect, Qt::AlignCenter, era_name);
}

void Era::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit editEra(this);
    }
}
