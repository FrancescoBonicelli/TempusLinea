#ifndef EVENTFORM_H
#define EVENTFORM_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QDateTimeEdit>

#include "event.h"
#include "category.h"

class EventForm : public QDialog
{
    Q_OBJECT

public:
    EventForm(const QString& title, std::vector<Category*>& categories, QWidget* parent);
    EventForm(const QString& title, Event* event, std::vector<Category*>& categories, QWidget* parent);

public slots:
    void verify();
    void deleteEvent();

public:
    QString name() const;
    QDate date() const;
    Category* category();

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* date_label;
    QDateEdit* date_value;
    QLabel* category_label;
    QComboBox* category_value;
    QDialogButtonBox* button_box;
    QPushButton* delete_event_button;
    std::vector<Category*> categories;
};

#endif // EVENTFORM_H
