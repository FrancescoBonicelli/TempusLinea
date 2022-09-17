#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QAction>

#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Canvas* canvas;

    bool loadCanvas();
    bool saveCanvas() const;
};
#endif // MAINWINDOW_H
