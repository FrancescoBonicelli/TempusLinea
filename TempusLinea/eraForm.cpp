#include <eraForm.h>

EraForm::EraForm(const QString& title, QWidget* parent)
    : QDialog(parent)
{
    name_label = new QLabel(tr("Name:"));
    name_value = new QLineEdit;
    starting_date_label = new QLabel(tr("Starting date:"));
    ending_date_label = new QLabel(tr("Ending date:"));
    starting_date_value = new QDateEdit();
    ending_date_value = new QDateEdit();
    color_label = new QLabel(tr("Color:"));
    color_value = new ColorPicker();

    delete_era_button = new QPushButton("Delete era");
    delete_era_button->setVisible(false);

    starting_date_value->setCalendarPopup(true);
    ending_date_value->setCalendarPopup(true);

    starting_date_value->setDate(QDate(QDate::currentDate().year(), 1, 1));
    ending_date_value->setDate(QDate(QDate::currentDate().year(), 12, 31));

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &EraForm::verify);
    connect(button_box, &QDialogButtonBox::rejected, this, &EraForm::reject);
    connect(delete_era_button, &QPushButton::clicked, this, &EraForm::deleteEra);

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
    mainLayout->addWidget(delete_era_button, 2, 0);
    setLayout(mainLayout);

    setWindowTitle(title);
}

EraForm::EraForm(const QString& title, Era* era, QWidget* parent)
    : EraForm(title, parent)
{
    name_value->setText(era->getName());
    starting_date_value->setDate(era->getStartingDate());
    ending_date_value->setDate(era->getEndingDate());
    color_value->setCurrentColor(era->getColor());

    delete_era_button->setVisible(true);
}

QString EraForm::name() const
{
    return name_value->text();
}

QDate EraForm::startingDate() const
{
    return starting_date_value->date();
}

QDate EraForm::endingDate() const
{
    return ending_date_value->date();
}

QColor EraForm::color() const
{
    return color_value->getCurrentColor();
}

void EraForm::verify()
{
    if (name_value->text().isEmpty() || !starting_date_value->date().isValid() || !ending_date_value->date().isValid()) {
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

void EraForm::deleteEra()
{
    name_value->setText("~");
    reject();
}