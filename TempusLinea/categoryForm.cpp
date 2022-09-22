#include <categoryForm.h>

CategoryForm::CategoryForm(const QString& title, QWidget* parent)
    : QDialog(parent)
{
    name_label = new QLabel(tr("Name:"));
    name_value = new QLineEdit;
    color_label = new QLabel(tr("Color:"));
    color_value = new ColorPicker();

    delete_category = new QPushButton("Delete category");
    delete_category->setVisible(false);

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &CategoryForm::verify);
    connect(button_box, &QDialogButtonBox::rejected, this, &CategoryForm::reject);
    connect(delete_category, SIGNAL(clicked()), this, SLOT(deleteCategory()));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(name_label, 0, 0);
    mainLayout->addWidget(name_value, 0, 1);
    mainLayout->addWidget(color_label, 0, 2);
    mainLayout->addWidget(color_value, 0, 3);
    mainLayout->addWidget(button_box, 1, 3);
    mainLayout->addWidget(delete_category, 1, 0);
    setLayout(mainLayout);

    setWindowTitle(title);
}

CategoryForm::CategoryForm(const QString& title, Category* category, QWidget* parent)
    : CategoryForm(title, parent)
{
    name_value->setText(category->getName());
    color_value->setCurrentColor(category->getColor());

    delete_category->setVisible(true);
}

QString CategoryForm::name() const
{
    return name_value->text();
}

QColor CategoryForm::color() const
{
    return color_value->getCurrentColor();
}

void CategoryForm::verify()
{
    if (name_value->text().isEmpty()) {
        QMessageBox::StandardButton answer;
        answer = QMessageBox::warning(this, tr("Incomplete Form"),
            tr("The form does not contain all the necessary information.\n"
                "Do you want to discard it?"),
            QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
            reject();
    }
    else
    {
        accept();
        return;
    }
}

void CategoryForm::deleteCategory()
{
    QMessageBox::StandardButton answer;
    answer = QMessageBox::warning(this, tr("Category Delation"),
        tr("The category with all its content will be deleted.\n"
            "Do you want to proceed?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
    {
        name_value->setText("~");
        reject();
    }
}