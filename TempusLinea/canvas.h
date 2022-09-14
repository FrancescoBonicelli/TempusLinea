#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonArray>

#include <array>
#include <vector>
#include <cmath>

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "era.h"
#include "mouseMenu.h"

#define MOUSE_MENU_SIZE 80

using namespace boost::gregorian;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    date canvas_start_date, canvas_end_date;
    int v_offset;

    QPoint starting_drag_position;
    bool dragging;

    std::vector<Era> eras_vector;

    int getDatePosition(date d);

    MouseMenu *mouse_menu;

signals:

};

#endif // CANVAS_H
