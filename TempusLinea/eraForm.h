#ifndef ERAFORM_H
#define ERAFORM_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QColor>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTimeEdit>

#include"conf.h"

#include "colorPicker.h"
#include "era.h"
#include "category.h"

class EraForm : public QDialog
{
    Q_OBJECT

public:
    EraForm(const QString& title, std::vector<Category*>& categories, QWidget* parent);
    EraForm(const QString& title, Era* era, std::vector<Category*>& categories, QWidget* parent);

public slots:
    void verify();
    void deleteEra();

public:
    QString name() const;
    QDate startingDate() const;
    QDate endingDate() const;
    QColor color() const;
    Category* category() const;

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* starting_date_label;
    QDateEdit* starting_date_value;
    QLabel* ending_date_label;
    QDateEdit* ending_date_value;
    QLabel* color_label;
    QLabel* category_label;
    ColorPicker* color_value;
    QComboBox* category_value;
    QDialogButtonBox* button_box;
    QPushButton* delete_era_button;
    std::vector<Category*> categories;
};

#endif // ERAFORM_H
