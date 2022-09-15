#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColorDialog>
#include <QColor>
#include <QPainter>
#include <QMouseEvent>

class ColorPicker : public QWidget
{
public:
    ColorPicker();

    QColor getCurrentColor();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QColor current_color;
};

#endif // COLORPICKER_H
