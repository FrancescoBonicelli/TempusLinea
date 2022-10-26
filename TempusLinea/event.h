#ifndef EVENT_H
#define EVENT_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class Event : public QWidget
{
    Q_OBJECT
public:
    Event(QWidget *parent = nullptr);
    Event(QString name, QDate date, QString& category, QWidget *parent = nullptr);

    void setName(QString name);
    void setDate(QDate date);
    void setCategory(QString &category);

    QString getName();
    QDate getDate();
    QString getCategory();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    QRect label_rect;

    QString toString();

signals:
    void editEvent(Event*);
    void showMessage(QString);

private:
    QString name;
    QDate date;
    QString *category;
    QLabel* label;

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // EVENT_H
