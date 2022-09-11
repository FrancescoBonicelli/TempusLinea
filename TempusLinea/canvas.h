#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <array>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::gregorian;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    date canvas_start, canvas_end;
    int v_offset;

    QPoint starting_drag_position;
    bool dragging;



signals:

};

#endif // CANVAS_H
