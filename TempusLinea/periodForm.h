#ifndef PERIODFORM_H
#define PERIODFORM_H

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

#include"conf.h"

#include "period.h"
#include "category.h"

class PeriodForm : public QDialog
{
    Q_OBJECT

public:
    PeriodForm(const QString& title, std::vector<Category*>& categories, QWidget* parent);
    PeriodForm(const QString& title, Period* period, std::vector<Category*>& categories, QWidget* parent);

public slots:
    void verify();
    void deletePeriod();

public:
    QString name() const;
    QDate starting_date() const;
    QDate ending_date() const;
    Category* category();

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* starting_date_label;
    QDateEdit* starting_date_value;
    QLabel* ending_date_label;
    QDateEdit* ending_date_value;
    QLabel* category_label;
    QComboBox* category_value;
    QDialogButtonBox* button_box;
    QPushButton* delete_period_button;
    std::vector<Category*> categories;
};

#endif // PERIODFORM_H
