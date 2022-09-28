#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class Event
{
public:
    Event();
    Event(QString name, QDate date, QString* category);

    void setName(QString name);
    void setDate(QDate date);
    void setCategory(QString &category);

    QString getName();
    QDate getDate();
    QString getCategory();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

private:
    QString name;
    QDate date;
    QString *category;
};

#endif // EVENT_H
