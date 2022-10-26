#include "period.h"

Period::Period(QWidget *parent)
    : QWidget{parent}
{
    label = new QLabel(this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    label->setAutoFillBackground(true);

    this->setLayout(layout);

    label_rect = QRect(0, 0, 0, 0);
    period_rect = QRect(0, 0, 0, 0);
}

Period::Period(QString name, QDate starting_date, QDate ending_date, QString& category, QWidget *parent)
    : Period{parent}
{
    this->name = name;
    this->starting_date = starting_date;
    this->ending_date = ending_date;
    this->category = &category;
    this->label->setText(name);
}

void Period::setName(QString name)
{
    this->name = name;
    this->label->setText(name);
}

void Period::setStartingDate(QDate starting_date)
{
    this->starting_date = starting_date;
}

void Period::setEndingDate(QDate ending_date)
{
    this->ending_date = ending_date;
}

void Period::setCategory(QString& category)
{
    this->category = &category;
}

QString Period::getName()
{
    return name;
}

QDate Period::getStartingDate()
{
    return starting_date;
}

QDate Period::getEndingDate()
{
    return ending_date;
}

QString Period::getCategory()
{
    return *category;
}

void Period::read(const QJsonObject& json)
{
    if(json.contains("period") && json["period"].isString())
    {
        QStringList dates = json["period"].toString().split(", ");
        starting_date = QDate::fromString(dates.first(), Qt::ISODate);
        ending_date = QDate::fromString(dates.last(), Qt::ISODate);
    }

    if(json.contains("name") && json["name"].isString())
    {
        name = json["name"].toString();
        this->label->setText(name);
    }
}

void Period::write(QJsonObject& json) const
{
    json["period"] = starting_date.toString(Qt::ISODate) + ", " + ending_date.toString(Qt::ISODate);
    json["name"] = name;
}

QString Period::toString()
{
    return "Period: " + name + ( category->isEmpty() ? "" : ("          Category: " + *category)) +
        "          Dates: " + starting_date.toString(Qt::ISODate) + ", " + ending_date.toString(Qt::ISODate);
}

void Period::mouseDoubleClickEvent(QMouseEvent* period)
{
    if (period->button() == Qt::LeftButton)
    {
        emit editPeriod(this);
    }
}

void Period::enterEvent(QEnterEvent *event)
{
    emit showMessage(this->toString());
}

void Period::leaveEvent(QEvent *event)
{
    emit showMessage("");
}
