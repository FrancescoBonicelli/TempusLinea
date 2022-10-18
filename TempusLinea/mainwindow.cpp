#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("TempusLinea"));
    setWindowIcon(QIcon("TempusLinea.ico"));

    canvas = new Canvas(this);
    setCentralWidget(canvas);

    left_menu = new LeftMenu(this);
    left_menu->setGeometry(QRect(QPoint(0, 0), QPoint(120, height())));
    left_menu->setVisible(false);

    left_menu_toggle_button = new LeftMenuToggler(this);
    left_menu_toggle_button->setGeometry(QRect(QPoint(10, 10), QPoint(60, 60)));

    connect(left_menu, &LeftMenu::newCanvasButtonClicked, this, &MainWindow::newCanvasSlot);
    connect(left_menu, &LeftMenu::saveCanvasButtonClicked, this, &MainWindow::saveCanvasSlot);
    connect(left_menu, &LeftMenu::saveAsCanvasButtonClicked, this, &MainWindow::saveAsCanvasSlot);
    connect(left_menu, &LeftMenu::loadCanvasButtonClicked, this, &MainWindow::loadCanvasSlot);
    connect(left_menu, &LeftMenu::exportCanvasButtonClicked, this, &MainWindow::exportCanvasSlot);
    connect(left_menu_toggle_button, &LeftMenuToggler::clicked, [this](){left_menu->setVisible(!left_menu->isVisible());});
    connect(canvas, &Canvas::mousePress, [this](){left_menu->setVisible(false);});
    connect(canvas, &Canvas::mouseWheel, [this](){left_menu->setVisible(false);});
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

bool MainWindow::loadCanvas()
{
    QString file_filter = tr("JSON (*.json)");
    QString file_name = QFileDialog::getOpenFileName(0,
        "Load Canvas",
        QDir::homePath(),
        tr("All files (*.*);;JSON (*.json)"),
        &file_filter);

    QFile load_file(file_name);

    if (!load_file.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc(QJsonDocument::fromJson(save_data));
    canvas->read(load_doc.object());
    load_file.close();

    current_file_name = file_name;
    setTitle();

    return true;
}

void MainWindow::setTitle()
{
    QString window_title = "TempusLinea - " + current_file_name.mid(current_file_name.lastIndexOf("/")+1);
    setWindowTitle(window_title);
}

bool MainWindow::saveCanvas()
{
    if (current_file_name.isEmpty())
    {
        QString file_filter = tr("JSON (*.json)");
        QString file_name = QFileDialog::getSaveFileName(0,
            "Save Canvas",
            QDir::homePath(),
            tr("All files (*.*);;JSON (*.json)"),
            &file_filter);

        if (file_name.isEmpty())
        {
            qWarning("Empty file name.");
            return false;
        }
        if(!file_name.endsWith(".json")) file_name += ".json";

        current_file_name = file_name;
        setTitle();
    }

    QFile save_file(current_file_name);

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

void MainWindow::newCanvasSlot()
{
    canvas->resetCanvas();
    canvas->restoreDefaults();
    canvas->update();

    left_menu->setVisible(false);
}

void MainWindow::saveCanvasSlot()
{
    if(saveCanvas())
    {
        left_menu->setVisible(false);
    }
}

void MainWindow::saveAsCanvasSlot()
{
    current_file_name = QString();
    saveCanvasSlot();
}

void MainWindow::loadCanvasSlot()
{
    if (loadCanvas())
    {
        left_menu->setVisible(false);
    }
}

void MainWindow::exportCanvasSlot()
{
    QString file_filter = tr("Images (*.bmp *.jpg *.jpeg *.png)");
    QString file_name = QFileDialog::getSaveFileName(0,
        "Save Canvas",
        "",
        tr("All files (*.*);;Images (*.bmp *.jpg *.jpeg *.png)"),
        &file_filter);

    if (!file_name.endsWith(".bmp") && !file_name.endsWith(".jpg") && 
        !file_name.endsWith(".jpeg") && !file_name.endsWith(".png")) file_name += ".jpeg";

    if (!file_name.isEmpty())
    {
        QPixmap pixmap(canvas->size());
        canvas->render(&pixmap);
        pixmap.save(file_name);
    }

    left_menu->setVisible(false);
}
