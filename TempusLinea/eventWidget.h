#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include <QLabel>

#include "event.h"

class EventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventWidget(QWidget *parent = nullptr);
    explicit EventWidget(Event* event, QWidget *parent = nullptr);

signals:
    void editEvent(Event*);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    Event *event;

    QLabel* label;
};

#endif // EVENTWIDGET_H
