#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

#include <array>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int range_begin, range_end;
    int v_offset;



signals:

};

#endif // CANVAS_H
