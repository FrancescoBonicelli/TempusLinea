#include "categoriesManager.h"

CategoriesManagerLine::CategoriesManagerLine(Category* category, QWidget *parent)
    : QWidget{parent}
{
    this->category = category;

    check_box = new QCheckBox();
    check_box->setChecked(category->isVisible());

    QWidget* rect = new QWidget();
    QPalette p(palette());
    p.setColor(QPalette::Window, category->getColor());
    rect->setAutoFillBackground(true);
    rect->setPalette(p);
    rect->setFixedSize(15, 15);

    label = new QLabel(category->getName());

    layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    layout->addWidget(check_box);
    layout->addWidget(rect);
    layout->addWidget(label);
}

Category* CategoriesManagerLine::getCategory()
{
    return category;
}

void CategoriesManagerLine::mouseDoubleClickEvent(QMouseEvent *event)
{
    CategoryForm dialog("Edit Category", category, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        category->setName(dialog.name());
        category->setColor(dialog.color());

        label->setText(dialog.name());
    }
    else if(dialog.name() == "~")
    {
        emit deleteCategory(category);
    }
}


CategoriesManager::CategoriesManager(QWidget *parent)
    : QWidget{parent}
{
    QLabel* title = new QLabel("Categories:");
    QFont font;
    font.setBold(true);
    title->setFont(font);

    layout = new QVBoxLayout();
    setLayout(layout);

    layout->addWidget(title);

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

        drawCategoryLine(new_category);
    }
}

void CategoriesManager::deleteCategory(Category* category)
{
    for(int i = 0; i < lines.size(); i++)
    {
        if(lines.at(i)->getCategory() == category)
        {
            lines.at(i)->close();
            lines.erase(lines.begin() + i);
        }
    }

    for(int i = 0; i < categories->size(); i++)
    {
        if(categories->at(i) == category)
        {
            delete categories->at(i);
            categories->erase(categories->begin() + i);
        }
    }

    resize(CATEGORIES_MANAGER_WIDTH, 35 + 20 * lines.size());
}

void CategoriesManager::refreshCategories()
{
    for (CategoriesManagerLine* line : lines) delete(line);
    lines.clear();
    for (int category_index = 0; category_index < categories->size(); ++category_index) {
        drawCategoryLine(categories->at(category_index));
    }
}

void CategoriesManager::drawCategoryLine(Category* c)
{
    CategoriesManagerLine* line = new CategoriesManagerLine(c);
    layout->addWidget(line);
    line->show();

    connect(line, SIGNAL(deleteCategory(Category*)), this, SLOT(deleteCategory(Category*)));

    lines.push_back(line);

    resize(CATEGORIES_MANAGER_WIDTH, 35 + 20 * lines.size());
}
