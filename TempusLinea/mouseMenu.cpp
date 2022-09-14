#include "mouseMenu.h"

MouseMenuButton::MouseMenuButton(QWidget *parent) : QWidget{parent}
{

}

void MouseMenuButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPoint starting_point(5, 5);
    QPoint ending_point(width()-5, height()-5);

    painter.fillRect(QRect(starting_point, ending_point), QColor(0, 0, 0, 100));
}

void MouseMenuButton::resizeEvent(QResizeEvent *event)
{
    repaint();
}


MouseMenu::MouseMenu(QWidget *parent) : QWidget{parent}
{
    new_era_button = new MouseMenuButton(this);
    new_era_button->setGeometry(QRect(0, 0, width()/2, height()/2));
    new_event_button = new MouseMenuButton(this);
    new_event_button->setGeometry(QRect(width()/2, 0, width()/2, height()/2));
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
