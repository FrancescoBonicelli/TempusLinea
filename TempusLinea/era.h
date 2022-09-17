#ifndef ERA_H
#define ERA_H

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QJsonObject>

using namespace boost::gregorian;

class Era : public QWidget
{
    Q_OBJECT
public:
    Era();
    Era(const Era&);
    explicit Era(QString, date starting_date, date ending_date, QColor color, QWidget* parent = nullptr);
    ~Era();

    date_period getPeriod();
    date getStartingDate();
    date getEndingDate();

    void setName(QString name);
    QString getName();

    void setColor(QColor color);
    QColor getColor();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

signals:
    void editEra(Era* era);

private:
    date_period* era_period;
    QString era_name;
    QColor era_color;

    QWidget* era_parent = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // ERA_H
