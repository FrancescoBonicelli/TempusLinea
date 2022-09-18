#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("TempusLinea"));

    canvas = new Canvas(this);
    setCentralWidget(canvas);

    left_menu = new LeftMenu(this);
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(width()/8, height())));

    connect(left_menu, SIGNAL(saveCanvasButtonClicked()), this, SLOT(saveCanvasSlot()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(width()/8, height())));
}

MainWindow::~MainWindow()
{
    delete canvas;
}

bool MainWindow::loadCanvas()
{
    QFile load_file(QStringLiteral("save.json"));

    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc(QJsonDocument::fromJson(save_data));
    canvas->read(load_doc.object());

    return true;
}

bool MainWindow::saveCanvas() const
{
    QFile save_file(QStringLiteral("save.json"));

    if (!save_file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject canvas_obj;
    canvas->write(canvas_obj);
    save_file.write(QJsonDocument(canvas_obj).toJson());

    return true;
}

void MainWindow::saveCanvasSlot()
{
    saveCanvas();
}
