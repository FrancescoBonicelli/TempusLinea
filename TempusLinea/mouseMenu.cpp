#include "mouseMenu.h"

MouseMenuButton::MouseMenuButton(QWidget *parent, QString text) : QWidget{parent}
{
    btn_text = text;
}

void MouseMenuButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPoint starting_point(5, 5);
    QPoint ending_point(width()-5, height()-5);

    QRect rect = QRect(starting_point, ending_point);

    painter.drawRoundedRect(rect, 5, 5);
    painter.drawText(rect, Qt::AlignCenter, btn_text);
}

void MouseMenuButton::resizeEvent(QResizeEvent *event)
{
    repaint();
}

void MouseMenuButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}


MouseMenu::MouseMenu(QWidget *parent) : QWidget{parent}
{
    new_era_button = new MouseMenuButton(this, "New\nERA");
    new_era_button->setGeometry(QRect(0, 0, width()/2, height()/2));
    new_event_button = new MouseMenuButton(this, "New\nEVENT");
    new_event_button->setGeometry(QRect(width()/2, 0, width()/2, height()/2));

    connect(new_era_button, SIGNAL(clicked()), this, SLOT(createEra()));
}

void MouseMenu::resizeEvent(QResizeEvent *event)
{
    new_era_button->setGeometry(QRect(0, 0, width()/2, height()/2));
    new_event_button->setGeometry(QRect(width()/2, 0, width()/2, height()/2));
}

void MouseMenu::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       setVisible(false);
    }
}

void MouseMenu::createEra()
{
    setVisible(false);
    emit newEraClicked();
}

void MouseMenu::createEvent()
{
    setVisible(false);
    emit newEventClicked();
}
