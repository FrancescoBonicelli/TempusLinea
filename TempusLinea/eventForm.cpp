#include "eventForm.h"

EventForm::EventForm(const QString& title, std::vector<Category*>& categories, QWidget* parent)
    : QDialog(parent)
{
    this->categories = categories;
    name_label = new QLabel(tr("Name:"));
    name_value = new QLineEdit;
    date_label = new QLabel(tr("Date:"));
    date_value = new QDateEdit();
    category_label = new QLabel(tr("Category:"));
    category_value = new QComboBox();
    for (Category* c : categories)
    {
        category_value->addItem(c->getName());
    }

    delete_event_button = new QPushButton("Delete event");
    delete_event_button->setVisible(false);

    date_value->setCalendarPopup(true);
    date_value->setMinimumDate(min_date);

    date_value->setDate(QDate(QDate::currentDate().year(), 7, 1));

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &EventForm::verify);
    connect(button_box, &QDialogButtonBox::rejected, this, &EventForm::reject);
    connect(delete_event_button, &QPushButton::clicked, this, &EventForm::deleteEvent);

    QFormLayout* mainLayout = new QFormLayout;
    mainLayout->addRow(name_label, name_value);
    mainLayout->addRow(date_label, date_value);
    mainLayout->addRow(category_label, category_value);
    mainLayout->addRow(delete_event_button, button_box);
    setLayout(mainLayout);

    setWindowTitle(title);
}

EventForm::EventForm(const QString& title, Event* event, std::vector<Category*>& categories, QWidget* parent)
    : EventForm(title, categories, parent)
{
    name_value->setText(event->getName());
    date_value->setDate(event->getDate());
    category_value->setCurrentText(event->getCategory());

    delete_event_button->setVisible(true);
}

QString EventForm::name() const
{
    return name_value->text();
}

QDate EventForm::date() const
{
    return date_value->date();
}

Category* EventForm::category()
{
    return categories.at(category_value->currentIndex());
}

void EventForm::verify()
{
    if (name_value->text().isEmpty() || !date_value->date().isValid())
    {
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

void EventForm::deleteEvent()
{
    name_value->setText("~");
    reject();
}