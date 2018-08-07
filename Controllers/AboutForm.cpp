#include "AboutForm.h"
#include "ui_AboutForm.h"

AboutForm::AboutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutForm)
{
    ui->setupUi(this);

    QPixmap pixmap(Parameters::iconLocation);
    // make a circle image
    QBitmap mask(pixmap.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), mask.width()/1.5, mask.height()/1.5);
    pixmap.setMask(mask);
    ui->app_logo->setPixmap(pixmap);
    ui->app_logo->setScaledContents(true);

    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint |Qt::WindowType::Window);
}

AboutForm::~AboutForm()
{
    delete ui;
}

void AboutForm::on_bt_ok_clicked()
{
    this->hide();
}
