#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <Config/Parameters.h>

namespace Ui {
class AboutForm;
}

class AboutForm : public QWidget
{
    Q_OBJECT

public:
    explicit AboutForm(QWidget *parent = 0);
    ~AboutForm();

private slots:
    void on_bt_ok_clicked();

private:
    Ui::AboutForm *ui;
};

#endif // ABOUTFORM_H
