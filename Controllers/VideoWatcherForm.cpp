#include "VideoWatcherForm.h"
#include "ui_VideoWatcherForm.h"

VideoWatcherForm::VideoWatcherForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoWatcherForm)
{
    ui->setupUi(this);
}

VideoWatcherForm::~VideoWatcherForm()
{
    delete ui;
}
