#ifndef MOUSEMENU_H
#define MOUSEMENU_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>

class MouseMenuButton : public QWidget
{
    Q_OBJECT
public:
    explicit MouseMenuButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};


class MouseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MouseMenu(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    MouseMenuButton *new_era_button, *new_event_button;

};

#endif // MOUSEMENU_H
