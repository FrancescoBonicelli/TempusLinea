#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("TempusLinea"));

    canvas = new Canvas(this);
    setCentralWidget(canvas);

    left_menu = new LeftMenu(this);
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(width()/8, height())));

    connect(left_menu, SIGNAL(saveCanvasButtonClicked()), this, SLOT(saveCanvasSlot()));
    connect(left_menu, SIGNAL(loadCanvasButtonClicked()), this, SLOT(loadCanvasSlot()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(width()/8, height())));
}

MainWindow::~MainWindow()
{
    delete canvas;
}

bool MainWindow::loadCanvas(QString file_name)
{
    QFile load_file(file_name);

    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc(QJsonDocument::fromJson(save_data));
    canvas->read(load_doc.object());
    load_file.close();

    return true;
}

bool MainWindow::saveCanvas(QString file_name)
{
    if(!file_name.endsWith(".json")) file_name += ".json";
    QFile save_file(file_name);

    if (!save_file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject canvas_obj;
    canvas->write(canvas_obj);
    save_file.write(QJsonDocument(canvas_obj).toJson());
    save_file.close();

    return true;
}

void MainWindow::saveCanvasSlot()
{
    QString file_filter = tr("JSON (*.json)");
    QString file_name = QFileDialog::getSaveFileName(0,
                                                     "Save Canvas",
                                                     "",
                                                     tr("All files (*.*);;JSON (*.json)"),
                                                     &file_filter);

    if(!file_name.isEmpty())
        saveCanvas(file_name);
}

void MainWindow::loadCanvasSlot()
{
    QString file_filter = tr("JSON (*.json)");
    QString file_name = QFileDialog::getOpenFileName(0,
                                                     "Load Canvas",
                                                     "",
                                                     tr("All files (*.*);;JSON (*.json)"),
                                                     &file_filter);
    loadCanvas(file_name);
}
