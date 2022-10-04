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

#include "era.h"
#include "eraForm.h"
#include "mouseMenu.h"
#include "category.h"
#include "categoriesManager.h"
#include "event.h"
#include "eventForm.h"
#include "period.h"
#include "periodForm.h"

#define MOUSE_MENU_SIZE 100

#define EVENT_LABEL_HEIGHT 20
#define EVENT_LABEL_V_SPACING 10

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

signals:
    void mousePress();
    void mouseWheel();

public slots:
    void openEraCreationDialog();
    void openEraEditDialog(Era* era);
    void openEventCreationDialog();
    void openEventEditDialog(Event* event);
    void openPeriodCreationDialog();
    void openPeriodEditDialog(Period* period);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QDate canvas_start_date, canvas_end_date;
    int v_offset;

    std::vector<Era*> eras_vector;
    std::vector<Category*> categories;
    CategoriesManager* categories_manager;

    QPoint starting_drag_position;
    bool dragging;

    int getDatePosition(QDate d);
    void placeEvents(std::vector<Event*> events_vector, QFontMetrics fm);

    MouseMenu *mouse_menu;
};

#endif // CANVAS_H
