#ifndef ERAFORM_H
#define ERAFORM_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QColor>
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>

#include "colorPicker.h"

class EraForm : public QDialog
{
    Q_OBJECT

public:
    EraForm(const QString& title, QWidget* parent);

public slots:
    void verify();

public:
    QString name() const;
    QDate startingDate() const;
    QDate endingDate() const;
    QColor color() const;

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* starting_date_label;
    QCalendarWidget* starting_date_value;
    QLabel* ending_date_label;
    QCalendarWidget* ending_date_value;
    QLabel* color_label;
    ColorPicker* color_value;
    QDialogButtonBox* button_box;
};

#endif // ERAFORM_H
