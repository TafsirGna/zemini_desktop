#include "Controllers\PreferencesForm.h"
#include "ui_preferencesform.h"
#include <QDesktopWidget>

ZeminiPreferencesForm::ZeminiPreferencesForm(QList<AbstractManager> * listEntitiesManager,QWidget *parent,QSqlDatabase * db) : QWidget(parent), ui(new Ui::ZeminiPreferencesForm)
{
    ui->setupUi(this);

    //setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint /*| Qt::WindowCloseButtonHint */ );

    // This commands put the form on the middle of the screen
    QDesktopWidget bureau;
    QRect surface_bureau = bureau.screenGeometry();
    //int x = surface_bureau.width()/2 - width()/2;
    //int y = surface_bureau.height()/2 - height()/2;
    int x = surface_bureau.width()-(width()+90), y = surface_bureau.height()-(height()+100);
    move(x,y);

    /*Empêcher le redimensionnement de la fenêtre*/
    setMinimumSize(size());
    setMaximumSize(size());

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(handle_current_tab_changed(int)));
    ui->tabWidget->setCurrentIndex(0);

    // here, i retrieve all the managers, i've passed as arguments to this object
    typeManager = (TypeManager *) &(listEntitiesManager->at(1));
    userManager = (UserManager *) &(listEntitiesManager->at(4));
    //dirManager = (DirectoryManager *) &(listEntitiesManager->at(2));

    // here's the code that calculates the percentage of each type and shows it under the statistics tab
    /*
    ui->progressBarMovies->setValue(typeManager->getTypePercentage("Movies"));
    ui->progressBarSeries->setValue(typeManager->getTypePercentage("Series"));
    ui->progressBarOthers->setValue(typeManager->getTypePercentage("Others"));
    ui->progressBarSoftwares->setValue(typeManager->getTypePercentage("Softwares"));
    ui->progressBarSongs->setValue(typeManager->getTypePercentage("Songs"));
    */

    // And here, i fill the email field with user's email
    ui->le_email->setText(userManager->getUser()->getEmail());

    // i disable for now the checkbox intended to set up the 'on start' launch of the application
    ui->chkbx_launchOnStart->setEnabled(false);

    // i fill the combobox about languages with all the languages supported
    ui->cb_language->addItem("French");

    // i fill the other location lineedit with its content if it exists
    /*
    QList<Directory> dirsList = dirManager->findDirectoryByIdDirectory(0);
    Directory * dir = (Directory *) &(dirsList.at(0));

    otherDir = dirManager->findOtherDirectoryOnDisk();
    if (otherDir != NULL)
        ui->le_otherLocation->setText(otherDir->getPath());
        */
}

ZeminiPreferencesForm::~ZeminiPreferencesForm()
{
    delete ui;
}

void ZeminiPreferencesForm::on_bt_ok_clicked()
{
    this->hide();
}

void ZeminiPreferencesForm::on_bt_cancel_clicked()
{
    this->hide();
}

void ZeminiPreferencesForm::on_bt_apply_clicked()
{
    int response = QMessageBox::warning(this, "Confirmation" , "Validez-vous les modifications que vous souhaitez appliquer ?", QMessageBox::Yes, QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        // i apply the modifications done on the tab with index 0

        // i apply the modifications done on the tab with index 1
        if (ui->chkbx_updatecompte->isChecked())
        {
            QString ol_text = ui->le_otherLocation->text();

            QStringList stringList;
            stringList << ol_text;
            stringList << ol_text+"/Movies";
            stringList << ol_text+"/Series";
            stringList << ol_text+"/Songs";
            stringList << ol_text+"/Softwares";

            /*
            if (otherDir != NULL)
            {
                if (ol_text != otherDir->getPath())
                {
                    emit setupDirectory(stringList);
                }
            }
            else
            {
                qDebug() << "Start enumerating";
                emit setupDirectory(stringList);
            }
            */
        }
    }
    this->hide();
}

void ZeminiPreferencesForm::on_bt_otherLocation_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this);
    if (!folderName.isEmpty())
    {
        ui->le_otherLocation->setText(folderName);
    }
    //QDesktopServices::openUrl(QUrl("file:///"+fileName, QUrl::TolerantMode));
}

void ZeminiPreferencesForm::on_chkbx_updatecompte_clicked(bool checked)
{
    if (checked)
    {
        ui->lb_otherLocation->setEnabled(true);

        ui->le_otherLocation->setEnabled(true);
        ui->le_otherLocation->setReadOnly(true);

        ui->bt_otherLocation->setEnabled(true);
    }
    else
    {
        ui->lb_otherLocation->setEnabled(false);
        //ui->le_otherLocation->setText("");
        ui->le_otherLocation->setEnabled(false);
        ui->bt_otherLocation->setEnabled(false);
    }
}

void ZeminiPreferencesForm::handle_current_tab_changed(int position)
{
    switch (position)
    {
    case 0:
        ui->bt_ok->setVisible(true);
        ui->bt_apply->setVisible(true);
        break;

    case 1:
        ui->bt_ok->setVisible(true);
        ui->bt_apply->setVisible(true);
        break;

    case 2:

        //i hide some buttons
        ui->bt_ok->setVisible(false);
        ui->bt_apply->setVisible(false);
        break;
    }
}
