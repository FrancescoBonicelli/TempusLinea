#include "era.h"

Era::Era()
{
}

Era::Era(const Era& era) : QWidget{ era.era_parent }
{
    era_name = era.era_name;
    era_period = era.era_period;
    era_color = era.era_color;
    setVisible(false);
}

Era::Era(QString name, date starting_date, date ending_date, QColor color, QWidget* parent) : QWidget{ parent }
{
    era_period = new date_period(starting_date, ending_date);
    era_name = name;
    era_color = color;

    era_parent = parent;
    setVisible(false);
}

Era::~Era()
{
    //delete era_period;
}

date_period Era::getPeriod()
{
    return *era_period;
}

date Era::getStartingDate()
{
    return era_period->begin();
}

date Era::getEndingDate()
{
    return era_period->end();
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
        QStringList dates = json["era_period"].toString().remove('[').remove(']').split('/');

        era_period = new date_period(
            from_string(dates.first().toStdString()),
            from_string(dates.last().toStdString()));
    }
    if (json.contains("era_name") && json["era_name"].isString())
        era_name = json["era_name"].toString();
    if (json.contains("era_color") && json["era_color"].isString())
        era_color = json["era_color"].toString();
}

void Era::write(QJsonObject& json) const
{
    json["era_period"] = QString::fromStdString(to_simple_string(*era_period));
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

void Era::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit editEra(this);
    }
}