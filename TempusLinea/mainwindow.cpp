#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QMenu* fileMenu = new QMenu(tr("&File"), this);
    QAction* new_action = fileMenu->addAction(tr("&New..."));
    menuBar()->addMenu(fileMenu);
    connect(new_action, &QAction::triggered, this, &MainWindow::openDialog);

    canvas = new Canvas(this);
    setCentralWidget(canvas);
    setWindowTitle(tr("TempusLinea"));
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

void MainWindow::openDialog()
{
    EraForm dialog(tr("Era Details"), this);

    if (dialog.exec() == QDialog::Accepted) {
        canvas->eras_vector.push_back(Era(dialog.name(), qdate2date(dialog.startingDate()), qdate2date(dialog.endingDate()), dialog.color()));
    }
}

date MainWindow::qdate2date(QDate qdate)
{
    return date(qdate.year(), qdate.month(), qdate.day());
}