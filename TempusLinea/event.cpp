#include "event.h"

Event::Event(QWidget *parent)
    : QWidget{parent}
{
    label = new QLabel(this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(label);

    this->setLayout(layout);
}

Event::Event(QString name, QDate date, QString& category, QWidget *parent)
    : Event{parent}
{
    this->name = name;
    this->label->setText(name);
    this->date = date;
    this->category = &category;
}

void Event::setName(QString name)
{
    this->name = name;
}

void Event::setDate(QDate date)
{
    this->date = date;
}

void Event::setCategory(QString& category)
{
    this->category = &category;
}

QString Event::getName()
{
    return name;
}

QDate Event::getDate()
{
    return date;
}

QString Event::getCategory()
{
    return *category;
}

void Event::read(const QJsonObject& json)
{
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    if (json.contains("date") && json["date"].isString())
        date = QDate::fromString(json["date"].toString(), Qt::ISODate);
}

void Event::write(QJsonObject& json) const
{
    json["name"] = name;
    json["date"] = date.toString(Qt::ISODate);
}
