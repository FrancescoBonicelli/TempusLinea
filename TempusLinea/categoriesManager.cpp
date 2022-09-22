#include "categoriesManager.h"

CategoriesManager::CategoriesManager(QWidget *parent)
    : QWidget{parent}
{
    add_category_button = new QPushButton("+", this);
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

    // Legend background
    QPen pen(Qt::black);
    QPainterPath path;
    path.addRoundedRect(QRect(QPoint(2, 2), QPoint(width()-2, height()-2)), 5, 5);
    painter.setPen(pen);
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);

    // Legend title
    QFont font;
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);
    painter.setFont(font);
    painter.drawText(8, 22, "Categories:");
}

void CategoriesManager::resizeEvent(QResizeEvent *event)
{
    add_category_button->setGeometry(QRect(QPoint(width() - 28, 8), QPoint(width() - 10, 26)));
    emit resized();
}
