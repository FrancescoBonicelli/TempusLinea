#ifndef MOUSEMENU_H
#define MOUSEMENU_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>

class MouseMenuButton : public QWidget
{
    Q_OBJECT
public:
    explicit MouseMenuButton(QWidget *parent = nullptr, QString text = "");
    void setRotation(int angleDeg);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString btn_text;
    int rotation;
};


class MouseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MouseMenu(QWidget *parent = nullptr);

signals:
    void newEraClicked();
    void newEventClicked();
    void newPeriodClicked();

public slots:
    void createEra();
    void createEvent();
    void createPeriod();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    MouseMenuButton *new_era_button, *new_event_button, *new_period_button, *spare_button;

};

#endif // MOUSEMENU_H
