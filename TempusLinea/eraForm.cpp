#include<eraForm.h>

EraForm::EraForm(const QString& title, QWidget* parent)
    : QDialog(parent)
{
    name_label = new QLabel(tr("Name:"));
    name_value = new QLineEdit;
    starting_date_label = new QLabel(tr("Starting date:"));
    ending_date_label = new QLabel(tr("Ending date:"));
    starting_date_value = new QCalendarWidget();
    ending_date_value = new QCalendarWidget();
    color_label = new QLabel(tr("Color:"));
    color_value = new QColorDialog;

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &EraForm::verify);
    connect(button_box, &QDialogButtonBox::rejected, this, &EraForm::reject);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(name_label, 0, 0);
    mainLayout->addWidget(name_value, 0, 1);
    mainLayout->addWidget(color_label, 0, 2);
    mainLayout->addWidget(color_value, 0, 3);
    mainLayout->addWidget(starting_date_label, 1, 0);
    mainLayout->addWidget(starting_date_value, 1, 1);
    mainLayout->addWidget(ending_date_label, 1, 2);
    mainLayout->addWidget(ending_date_value, 1, 3);
    mainLayout->addWidget(button_box, 2, 3);
    setLayout(mainLayout);

    setWindowTitle(title);
}

QString EraForm::name() const
{
    return name_value->text();
}

QDate EraForm::startingDate() const
{
    return starting_date_value->selectedDate();
}

QDate EraForm::endingDate() const
{
    return ending_date_value->selectedDate();
}

QColor EraForm::color() const
{
    return color_value->selectedColor();
}

void EraForm::verify()
{
    if (!name_value->text().isEmpty() && starting_date_value->selectedDate().isValid() && ending_date_value->selectedDate().isValid()) {
        accept();
        return;
    }

    QMessageBox::StandardButton answer;
    answer = QMessageBox::warning(this, tr("Incomplete Form"),
        tr("The form does not contain all the necessary information.\n"
            "Do you want to discard it?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
        reject();
}