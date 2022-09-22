#include "categoriesManager.h"

CategoriesManager::CategoriesManager(QWidget *parent)
    : QWidget{parent}
{
    add_category_button = new QPushButton("+", this);
    add_category_button->setGeometry(QRect(QPoint(width()-30, 10), QPoint(width()-10, 30)));
}

CategoriesManager::CategoriesManager(std::vector<Category>& categories, QWidget *parent)
    : CategoriesManager{parent}
{
    this->categories = &categories;
}

void CategoriesManager::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black);
    QPainterPath path;
    path.addRoundedRect(QRect(QPoint(2, 2), QPoint(width()-2, height()-2)), 5, 5);
    painter.setPen(pen);
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);
}

void CategoriesManager::resizeEvent(QResizeEvent *event)
{
    add_category_button->setGeometry(QRect(QPoint(width()-30, 10), QPoint(width()-10, 30)));
    emit resized();
}
