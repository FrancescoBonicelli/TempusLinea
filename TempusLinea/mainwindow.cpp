#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("TempusLinea"));

    canvas = new Canvas(this);
    setCentralWidget(canvas);

    left_menu = new LeftMenu(this);
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(120, height())));
    left_menu->setVisible(false);

    left_menu_toggle_button = new LeftMenuToggler(this);
    left_menu_toggle_button->setGeometry(QRect(QPoint(10, 10), QPoint(width()/20, width()/20)));

    connect(left_menu, SIGNAL(saveCanvasButtonClicked()), this, SLOT(saveCanvasSlot()));
    connect(left_menu, SIGNAL(loadCanvasButtonClicked()), this, SLOT(loadCanvasSlot()));
    connect(left_menu_toggle_button, SIGNAL(clicked(bool)), this, SLOT(toggleLeftMenu()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(120, height())));
    left_menu_toggle_button->setGeometry(QRect(QPoint(10, 10), QPoint(60, 60)));
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

void MainWindow::toggleLeftMenu()
{
    left_menu->setVisible(!left_menu->isVisible());
}
