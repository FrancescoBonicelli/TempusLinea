#ifndef CATEGORIESMANAGER_H
#define CATEGORIESMANAGER_H

#include <vector>

#include <QWidget>
#include <QPainter>
#include <QPushButton>

#include "category.h"

class CategoriesManager : public QWidget
{
    Q_OBJECT
public:
    explicit CategoriesManager(QWidget *parent = nullptr);
    explicit CategoriesManager(std::vector<Category>& categories, QWidget *parent = nullptr);

signals:
    void resized();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<Category>* categories;
    QPushButton* add_category_button;
};

#endif // CATEGORIESMANAGER_H
