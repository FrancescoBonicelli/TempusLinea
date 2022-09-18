#include "leftMenu.h"

LeftMenuButton::LeftMenuButton(QWidget *parent)
    : QPushButton{parent}
{

}

LeftMenuButton::LeftMenuButton(QString text, QWidget * parent)
    : QPushButton{parent}
{
    button_text = text;
}

void LeftMenuButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = QRect(QPoint(0, 0), QPoint(width(), height()));
    painter.fillRect(rect, QColor(180, 180, 180));
    painter.drawText(rect, Qt::AlignCenter, button_text);
}

LeftMenu::LeftMenu(QWidget *parent)
    : QWidget{parent}
{
    canvas_save_button = new LeftMenuButton("Save Canvas");

    layout = new QVBoxLayout(this);
    setLayout(layout);

    layout->addWidget(canvas_save_button);

    connect(canvas_save_button, SIGNAL(clicked(bool)), this, SLOT(saveCanvas()));
}

void LeftMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(QRect(QPoint(0, 0), QPoint(width(), height())), QColor(200, 200, 200));
}

void LeftMenu::saveCanvas()
{
    emit saveCanvasButtonClicked();
}
