#include "colorPicker.h"

ColorPicker::ColorPicker(QWidget *parent) : QWidget{parent}
{
    current_color = QColor(0, 0, 255);
}

QColor ColorPicker::getCurrentColor()
{
    current_color.setAlpha(100);
    return current_color;
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPoint starting_point(0, 0);
    QPoint ending_point(width(), height());

    painter.fillRect(QRect(starting_point, ending_point), current_color);
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    // TODO: manage calcel btn
    current_color = QColorDialog::getColor();
}
