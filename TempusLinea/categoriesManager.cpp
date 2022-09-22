#include "categoriesManager.h"

CategoriesManager::CategoriesManager(QWidget *parent)
    : QWidget{parent}
{
    add_category_button = new QPushButton("+", this);
    add_category_button->setGeometry(QRect(QPoint(width()-50, 10), QPoint(width()-10, 50)));
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
    painter.setPen(QPen(Qt::black));

    painter.drawRoundedRect(QRect(QPoint(0, 0), QPoint(width(), height())), 5, 5);
}

void CategoriesManager::resizeEvent(QResizeEvent *event)
{
    add_category_button->setGeometry(QRect(QPoint(width()-50, 10), QPoint(width()-10, 50)));
}
