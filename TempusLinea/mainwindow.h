#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QAction>
#include <QFileDialog>

#include "canvas.h"
#include "leftMenu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newCanvasSlot();
    void saveCanvasSlot();
    void saveAsCanvasSlot();
    void loadCanvasSlot();
    void exportCanvasSlot();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Canvas* canvas;
    LeftMenu* left_menu;
    LeftMenuToggler* left_menu_toggle_button;
    QString current_file_name;

    bool saveCanvas();
    bool loadCanvas();
    void setTitle();
};
#endif // MAINWINDOW_H
