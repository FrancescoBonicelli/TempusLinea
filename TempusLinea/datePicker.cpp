#include "datePicker.h"

DatePicker::DatePicker(QWidget *parent) : QWidget{parent}
{
    selected_date = QDate::currentDate().addYears(5000);

    date_label = new QLabel(this);
    date_label->setText(selected_date.toString());

    calendar = new QCalendarWidget(this);
    calendar->hide();

    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(receiveDate(QDate)));
}

QDate DatePicker::getSelectedDate()
{
    return selected_date;
}

void DatePicker::mousePressEvent(QMouseEvent *event)
{
    calendar->setSelectedDate(selected_date);
    calendar->show();
}

void DatePicker::receiveDate(QDate date)
{
    selected_date = date;
    date_label->setText(selected_date.toString());
    calendar->hide();
}
