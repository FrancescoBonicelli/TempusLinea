#ifndef ERA_H
#define ERA_H

#include <string>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QJsonObject>

class Era : public QWidget
{
    Q_OBJECT
public:
    Era();
    Era(const Era&);
    explicit Era(QString, QDate starting_date, QDate ending_date, QColor color, QWidget* parent = nullptr);
    ~Era();

    QDate getStartingDate();
    QDate getEndingDate();

    void setName(QString name);
    QString getName();

    void setColor(QColor color);
    QColor getColor();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

signals:
    void editEra(Era* era);

private:
    QString era_name;
    QColor era_color;
    QDate era_starting_date;
    QDate era_ending_date;

    QWidget* era_parent = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // ERA_H
