#ifndef PERIOD_H
#define PERIOD_H

#include <QString>
#include <QDate>
#include <QJsonObject>

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

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    QString name;
    QDate starting_date, ending_date;
};

#endif // PERIOD_H
