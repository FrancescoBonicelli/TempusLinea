#ifndef PERIOD_H
#define PERIOD_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QLabel>
#include <QHBoxLayout>

class Period : public QWidget
{
    Q_OBJECT
public:
    Period(QWidget *parent = nullptr);
    Period(QString name, QDate starting_date, QDate ending_date, QString& category, QWidget *parent = nullptr);

    void setName(QString name);
    void setStartingDate(QDate starting_date);
    void setEndingDate(QDate ending_date);
    void setCategory(QString& category);

    QString getName();
    QDate getStartingDate();
    QDate getEndingDate();
    QString getCategory();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    QRect label_rect, period_rect;

private:
    QString name;
    QDate starting_date, ending_date;
    QString* category;
    QLabel *label;
};

#endif // PERIOD_H
