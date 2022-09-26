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
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = QRect(QPoint(5, 5), QPoint(width()-5, height()-5));
    painter.drawRoundedRect(rect, 5, 5);
    painter.drawText(rect, Qt::AlignCenter, button_text);
}

LeftMenu::LeftMenu(QWidget *parent)
    : QWidget{parent}
{
    canvas_save_button = new LeftMenuButton("Save Canvas");
    canvas_load_button = new LeftMenuButton("Load Canvas");
    canvas_export_button = new LeftMenuButton("Export Canvas");
    canvas_export_button->setToolTip("Export current view as image");

    layout = new QVBoxLayout(this);
    setLayout(layout);

    layout->addWidget(canvas_save_button);
    layout->addWidget(canvas_load_button);
    layout->addWidget(canvas_export_button);

    connect(canvas_save_button, &LeftMenuButton::clicked, this, &LeftMenu::saveCanvas);
    connect(canvas_load_button, &LeftMenuButton::clicked, this, &LeftMenu::loadCanvas);
    connect(canvas_export_button, &LeftMenuButton::clicked, this, &LeftMenu::exportCanvas);
}

void LeftMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    canvas_save_button->resize(canvas_save_button->width(), 50);
    canvas_load_button->resize(canvas_load_button->width(), 50);
    canvas_export_button->resize(canvas_export_button->width(), 50);
    painter.fillRect(QRect(QPoint(0, 0), QPoint(width(), height())), QColor(200, 200, 200));
}

void LeftMenu::saveCanvas()
{
    emit saveCanvasButtonClicked();
}

void LeftMenu::loadCanvas()
{
    emit loadCanvasButtonClicked();
}

void LeftMenu::exportCanvas()
{
    emit exportCanvasButtonClicked();
}

LeftMenuToggler::LeftMenuToggler(QWidget *parent)
    : QPushButton{parent}
{

}

void LeftMenuToggler::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QImage image("TempusLineaLogo_100.png");
    painter.drawImage(QRect(QPoint(0, 0), QPoint(width(), height())), image, QRect(QPoint(-2, -2), QPoint(102, 102)));
}
