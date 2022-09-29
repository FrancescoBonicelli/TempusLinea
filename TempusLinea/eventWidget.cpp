#include "eventWidget.h"

EventWidget::EventWidget(QWidget *parent)
    : QWidget{parent}
{

}

EventWidget::EventWidget(Event* event, QWidget *parent)
    : EventWidget{parent}
{
    this->event = event;

    label = new QLabel(event->getName(), this);
}

void EventWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    emit editEvent(this->event);
}
