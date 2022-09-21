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
    explicit Era(QWidget* parent = nullptr);
    explicit Era(const Era&);
    explicit Era(QString, QDate starting_date, QDate ending_date, QColor color, QWidget* parent = nullptr);
    ~Era();

    QDate getStartingDate();
    void setStartingDate(QDate starting_date);
    QDate getEndingDate();
    void setEndingDate(QDate ending_date);

    void setName(QString name);
    QString getName();

    void setColor(QColor color);
    QColor getColor();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

signals:
    void editEra(Era*);

private:
    QString name;
    QColor color;
    QDate starting_date;
    QDate ending_date;

    QWidget* parent = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // ERA_H
