#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QAction>

#include "canvas.h"
#include "leftMenu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveCanvasSlot();
    void loadCanvasSlot();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Canvas* canvas;
    LeftMenu* left_menu;

    bool loadCanvas();
    bool saveCanvas() const;
};
#endif // MAINWINDOW_H
