#ifndef CATEGORIESMANAGER_H
#define CATEGORIESMANAGER_H

#include <vector>

#include <QFile>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QPainterPath>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "category.h"
#include "categoryForm.h"

#define CATEGORIES_MANAGER_WIDTH 150
#define CATEGORIES_MANAGER_MARGINS QPoint(-10, 10)

class CategoriesManagerLine : public QWidget
{
    Q_OBJECT
public:
    explicit CategoriesManagerLine(Category *category, QWidget *parent = nullptr);

    Category* getCategory();

signals:
    void deleteCategory(Category*);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Category* category;

    QCheckBox *check_box;
    QWidget* color_box;
    QLabel *label;
    QHBoxLayout *layout;
};


class CategoriesManager : public QWidget
{
    Q_OBJECT
public:
    explicit CategoriesManager(QWidget *parent = nullptr);
    explicit CategoriesManager(std::vector<Category*>& categories, QWidget *parent = nullptr);
    void refreshCategories();

signals:
    void resized();

public slots:
        void createCategory();
        void deleteCategory(Category* category);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<Category*>* categories;
    QPushButton* add_category_button;
    QVBoxLayout* layout;

    std::vector<CategoriesManagerLine*> lines;

    QLabel *label;

    void drawCategoryLine(Category* category);
};

#endif // CATEGORIESMANAGER_H
