#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class Event
{
public:
    Event();
    Event(QString name, QDate date);

    void setName(QString name);
    void setDate(QDate date);

    QString getName();
    QDate getDate();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    QString name;
    QDate date;
};

#endif // EVENT_H
