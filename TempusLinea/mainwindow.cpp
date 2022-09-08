#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    canvas = new Canvas(this);
    setCentralWidget(canvas);
}

MainWindow::~MainWindow()
{
    delete canvas;
}

