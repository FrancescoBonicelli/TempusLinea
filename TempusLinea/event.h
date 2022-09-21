#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDate>

class Event
{
public:
    Event();
    Event(QString name, QDate date);

    void setName(QString name);
    void setDate(QDate date);

    QString getName();
    QDate getDate();

private:
    QString name;
    QDate date;
};

#endif // EVENT_H
