#ifndef LEFTMENU_H
#define LEFTMENU_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QVBoxLayout>

class LeftMenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit LeftMenuButton(QWidget *parent = nullptr);
    explicit LeftMenuButton(QString text, QWidget * parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString button_text;


};

class LeftMenu : public QWidget
{
    Q_OBJECT
public:
    explicit LeftMenu(QWidget *parent = nullptr);

signals:
    void saveCanvasButtonClicked();
    void loadCanvasButtonClicked();

public slots:
    void saveCanvas();
    void loadCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout* layout;

    LeftMenuButton* canvas_save_button;
    LeftMenuButton* canvas_load_button;
};

class LeftMenuToggler : public QPushButton
{
    Q_OBJECT
public:
    explicit LeftMenuToggler(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // LEFTMENU_H
