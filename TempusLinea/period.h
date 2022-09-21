#ifndef PERIOD_H
#define PERIOD_H

#include <QString>
#include <QDate>

class Period
{
public:
    Period();
    Period(QString name, QDate starting_date, QDate ending_date);

    void setName(QString name);
    void setStartingDate(QDate starting_date);
    void setEndingDate(QDate ending_date);

    QString getName();
    QDate getStartingDate();
    QDate getEndingDate();

private:
    QString name;
    QDate starting_date, ending_date;
};

#endif // PERIOD_H
