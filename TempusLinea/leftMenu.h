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

public slots:
    void saveCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout* layout;

    LeftMenuButton* canvas_save_button;

};

#endif // LEFTMENU_H
