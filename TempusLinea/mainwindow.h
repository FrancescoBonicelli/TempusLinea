#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QAction>

#include <QMenu>
#include <QMenuBar>

#include "canvas.h"
#include "eraForm.h"

using namespace boost::gregorian;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openDialog();

private:
    Canvas* canvas;

    bool loadCanvas();
    bool saveCanvas() const;

    date qdate2date(QDate qdate);
};
#endif // MAINWINDOW_H
