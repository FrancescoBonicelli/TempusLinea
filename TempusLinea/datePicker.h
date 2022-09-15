#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QCalendarWidget>

class DatePicker : public QWidget
{
    Q_OBJECT
public:
    explicit DatePicker(QWidget *parent = nullptr);

    QDate getSelectedDate();

public slots:
    void receiveDate(QDate date);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QDate selected_date;
    QLabel *date_label;
    QCalendarWidget *calendar;
};

#endif // DATEPICKER_H
