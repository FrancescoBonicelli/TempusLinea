#include "mouseMenu.h"

MouseMenuButton::MouseMenuButton(QWidget *parent, QString text) : QWidget{parent}
{
    btn_text = text;
    rotation = 0;
}

void MouseMenuButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint starting_point(5, 5);
    QPoint ending_point(width(), height());

    QRect rect = QRect(starting_point, ending_point);

    painter.translate(rect.center());
    painter.rotate(rotation);
    painter.translate(-rect.center());

    painter.drawArc(5, 5, (width() - 5) * 2, (height()-5) * 2, 92 * 16, 86 * 16);
    painter.drawArc(30, 30, (width() - 30) * 2, (height()-30) * 2, 92 * 16, 86 * 16);
    painter.drawLine(5, height()-1, 30, height()-1);
    painter.drawLine(width()-1, 5, width()-1, 30);
    painter.translate(rect.center()+QPoint(1,1));
    painter.rotate(rotation < 180 ? -45 : 135);
    painter.translate(-rect.center()-QPoint(1, 1));
    painter.drawText(rect, Qt::AlignCenter, btn_text);
}

void MouseMenuButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}

void MouseMenuButton::setRotation(int angleDeg)
{
    this->rotation = angleDeg;
}


MouseMenu::MouseMenu(QWidget *parent) : QWidget{parent}
{
    new_event_button = new MouseMenuButton(this, "EVENT");
    new_period_button = new MouseMenuButton(this, "PERIOD");
    new_era_button = new MouseMenuButton(this, "ERA");
    spare_button = new MouseMenuButton(this, "SPARE");

    connect(new_era_button, &MouseMenuButton::clicked, this, &MouseMenu::createEra);
    connect(new_event_button, &MouseMenuButton::clicked, this, &MouseMenu::createEvent);
    connect(new_period_button, &MouseMenuButton::clicked, this, &MouseMenu::createPeriod);
}

void MouseMenu::resizeEvent(QResizeEvent *event)
{
    new_event_button->setRotation(0);
    new_event_button->setGeometry(QRect(0, 0, width()/2, height()/2));
    new_period_button->setRotation(90);
    new_period_button->setGeometry(QRect(width()/2, 0, width()/2, height()/2));
    new_era_button->setRotation(270);
    new_era_button->setGeometry(QRect(0, height()/2, width()/2, height()/2));
    spare_button->setRotation(180);
    spare_button->setGeometry(QRect(width()/2, height()/2, width()/2, height()/2));
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

void MouseMenu::createPeriod()
{
    setVisible(false);
    emit newPeriodClicked();
}
