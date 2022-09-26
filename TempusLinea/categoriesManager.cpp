#include "categoriesManager.h"

CategoriesManagerLine::CategoriesManagerLine(Category* category, QWidget *parent)
    : QWidget{parent}
{
    this->category = category;

    label = new QLabel(category->getName());

    check_box = new QCheckBox();
    check_box->setChecked(category->isVisible());

    layout = new QHBoxLayout();
    setLayout(layout);

    layout->addWidget(check_box);
    layout->addWidget(label);
}


CategoriesManager::CategoriesManager(QWidget *parent)
    : QWidget{parent}
{
    add_category_button = new QPushButton("+", this);
    connect(add_category_button, &QPushButton::clicked, this, &CategoriesManager::createCategory);
}

CategoriesManager::CategoriesManager(std::vector<Category*>& categories, QWidget *parent)
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
    painter.save();
    QFont font;
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);
    painter.setFont(font);
    painter.drawText(8, 22, "Categories:");
    painter.restore();
}

void CategoriesManager::resizeEvent(QResizeEvent *event)
{
    add_category_button->setGeometry(QRect(QPoint(width() - 28, 8), QPoint(width() - 10, 26)));
    emit resized();
}

void CategoriesManager::createCategory()
{
    CategoryForm dialog(tr("New Category Details"), this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Category* new_category = new Category(dialog.name(), dialog.color());
        categories->push_back(new_category);

        CategoriesManagerLine *line = new CategoriesManagerLine(new_category, this);
        line->setGeometry(0, 22 + 20*lines.size(), CATEGORIES_MANAGER_WIDTH, 40);
        line->show();
        lines.push_back(line);
    }

    resize(CATEGORIES_MANAGER_WIDTH, 35 + 20 * lines.size());
}
