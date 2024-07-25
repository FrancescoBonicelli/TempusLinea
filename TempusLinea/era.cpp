#include "era.h"

Era::Era(QWidget* parent) : QWidget{ parent }
{
}

Era::Era(const Era& era) : QWidget{ era.parent }
{
    this->name = era.name;
    this->color = era.color;
    this->starting_date = era.starting_date;
    this->ending_date = era.ending_date;
}

Era::Era(QString name, QDate starting_date, QDate ending_date, QColor color, QString& category, QWidget* parent) : QWidget{ parent }
{
    this->name = name;
    this->color = color;
    this->starting_date = starting_date;
    this->ending_date = ending_date;
    this->category = &category;

    this->parent = parent;
}

Era::~Era()
{
    //delete period;
}

QDate Era::getStartingDate()
{
    return starting_date;
}

void Era::setStartingDate(QDate starting_date)
{
    this->starting_date = starting_date;
}

QDate Era::getEndingDate()
{
    return ending_date;
}

void Era::setEndingDate(QDate ending_date)
{
    this->ending_date = ending_date;
}

void Era::setName(QString name)
{
    this->name = name;
}

QString Era::getName()
{
    return name;
}

QString Era::getFullName()
{

    return category->isEmpty()? name : (*category + " - " + name);
}

void Era::setColor(QColor color)
{
    this->color = color;
}

QColor Era::getColor()
{
    return color;
}

QString Era::getCategory()
{
    return *category;
}

void Era::setCategory(QString& category)
{
    this->category = &category;
}

void Era::read(const QJsonObject& json)
{
    if (json.contains("period") && json["period"].isString())
    {
        QStringList dates = json["period"].toString().split(", ");
        starting_date = QDate::fromString(dates.first(), Qt::ISODate);
        ending_date = QDate::fromString(dates.last(), Qt::ISODate);
    }
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    if (json.contains("color") && json["color"].isString())
        color = json["color"].toString();
}

void Era::write(QJsonObject& json) const
{
    json["period"] = starting_date.toString(Qt::ISODate) + ", " + ending_date.toString(Qt::ISODate);
    json["name"] = name;
    json["color"] = color.name(QColor::HexArgb);
}

QString Era::toString()
{
    return "Era: " + name + "          Dates: " + starting_date.toString(Qt::ISODate) + ", " + ending_date.toString(Qt::ISODate) +
            ( category->isEmpty() ? "" : ("          Category: " + *category)) ;
}

void Era::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint starting_point(5, 5);
    QPoint ending_point(width()-5, height()-5);

    QRect rect = QRect(starting_point, ending_point);

    painter.drawRoundedRect(rect, 5, 5);
    painter.drawText(rect, Qt::AlignCenter, getFullName());
}

void Era::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit editEra(this);
    }
}

void Era::enterEvent(QEnterEvent *event)
{
    emit showMessage(this->toString());
}

void Era::leaveEvent(QEvent *event)
{
    emit showMessage("");
}
