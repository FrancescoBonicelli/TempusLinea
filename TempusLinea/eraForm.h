#ifndef ERAFORM_H
#define ERAFORM_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QColor>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTimeEdit>

#include "colorPicker.h"
#include <era.h>

class EraForm : public QDialog
{
    Q_OBJECT

public:
    EraForm(const QString& title, QWidget* parent);
    EraForm(const QString& title, Era* era, QWidget* parent);


public slots:
    void verify();
    void deleteEra();

public:
    QString name() const;
    QDate startingDate() const;
    QDate endingDate() const;
    QColor color() const;

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* starting_date_label;
    QDateEdit* starting_date_value;
    QLabel* ending_date_label;
    QDateEdit* ending_date_value;
    QLabel* color_label;
    ColorPicker* color_value;
    QDialogButtonBox* button_box;
    QPushButton* delete_era_button;
};

#endif // ERAFORM_H
