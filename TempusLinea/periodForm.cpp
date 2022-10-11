#include "periodForm.h"

PeriodForm::PeriodForm(const QString& title, std::vector<Category*>& categories, QWidget* parent)
    : QDialog(parent)
{
    this->categories = categories;
    name_label = new QLabel(tr("Name:"));
    name_value = new QLineEdit;
    starting_date_label = new QLabel(tr("Starting date:"));
    starting_date_value = new QDateEdit();
    ending_date_label = new QLabel(tr("Ending date:"));
    ending_date_value = new QDateEdit();
    category_label = new QLabel(tr("Category:"));
    category_value = new QComboBox();
    for (Category* c : categories)
    {
        category_value->addItem(c->getName());
    }

    delete_period_button = new QPushButton("Delete period");
    delete_period_button->setVisible(false);

    starting_date_value->setCalendarPopup(true);
    starting_date_value->setMinimumDate(min_date);
    ending_date_value->setCalendarPopup(true);
    ending_date_value->setMinimumDate(min_date);

    starting_date_value->setDate(QDate(QDate::currentDate().year(), 6, 30));
    ending_date_value->setDate(QDate(QDate::currentDate().year(), 7, 1));

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &PeriodForm::verify);
    connect(button_box, &QDialogButtonBox::rejected, this, &PeriodForm::reject);
    connect(delete_period_button, &QPushButton::clicked, this, &PeriodForm::deletePeriod);

    QFormLayout* mainLayout = new QFormLayout;
    mainLayout->addRow(name_label, name_value);
    mainLayout->addRow(starting_date_label, starting_date_value);
    mainLayout->addRow(ending_date_label, ending_date_value);
    mainLayout->addRow(category_label, category_value);
    mainLayout->addRow(delete_period_button, button_box);
    setLayout(mainLayout);

    setWindowTitle(title);
}

PeriodForm::PeriodForm(const QString& title, Period* period, std::vector<Category*>& categories, QWidget* parent)
    : PeriodForm(title, categories, parent)
{
    name_value->setText(period->getName());
    starting_date_value->setDate(period->getStartingDate());
    ending_date_value->setDate(period->getEndingDate());
    category_value->setCurrentText(period->getCategory());

    delete_period_button->setVisible(true);
}

QString PeriodForm::name() const
{
    return name_value->text();
}

QDate PeriodForm::starting_date() const
{
    return starting_date_value->date();
}

QDate PeriodForm::ending_date() const
{
    return ending_date_value->date();
}

Category* PeriodForm::category()
{
    return categories.at(category_value->currentIndex());
}

void PeriodForm::verify()
{
    if (name_value->text().isEmpty() || !starting_date_value->date().isValid() || !ending_date_value->date().isValid())
    {
        QMessageBox::StandardButton answer;
        answer = QMessageBox::warning(this, tr("Incomplete Form"),
            tr("The form does not contain all the necessary information.\n"
                "Do you want to discard it?"),
            QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
            reject();
    }
    else if (starting_date_value->date().daysTo(ending_date_value->date()) <= 0)
    {
        QMessageBox::StandardButton answer;
        answer = QMessageBox::warning(this, tr("Dates Error"),
            tr("The ending date is prior to the starting date.\n"
                "Do you want to invert them?"),
            QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
        {
            QDate tmp = starting_date_value->date();
            starting_date_value->setDate(ending_date_value->date());
            ending_date_value->setDate(tmp);
        }
    }
    else
    {
        accept();
        return;
    }
}

void PeriodForm::deletePeriod()
{
    name_value->setText("~");
    reject();
}