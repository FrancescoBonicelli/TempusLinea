#ifndef CATEGORYFORM_H
#define CATEGORYFORM_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QColor>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTimeEdit>

#include "colorPicker.h"
#include <category.h>

class CategoryForm : public QDialog
{
    Q_OBJECT

public:
    CategoryForm(const QString& title, QWidget* parent);
    CategoryForm(const QString& title, Category* category, QWidget* parent);


public slots:
    void verify();
    void deleteCategory();

public:
    QString name() const;
    QColor color() const;

private:
    QLabel* name_label;
    QLineEdit* name_value;
    QLabel* color_label;
    ColorPicker* color_value;
    QDialogButtonBox* button_box;
    QPushButton* delete_category;
};

#endif // CATEGORYFORM_H
