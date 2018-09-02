#include "Controllers\PreferencesForm.h"
#include "ui_preferencesform.h"
#include <QDesktopWidget>

PreferencesForm::PreferencesForm(QWidget *parent, ServiceContainer * serviceContainer) : QWidget(parent), ui(new Ui::PreferencesForm)
{
    ui->setupUi(this);


    // Variables initialization
    AppData * pullInAppData = AppDataManager::getByKey("pullInDataFrequency");
    AppData * sendOutAppData = AppDataManager::getByKey("sendOutDataFrequency");

    //setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // This commands put the form on the middle of the screen
    QDesktopWidget bureau;
    QRect surface_bureau = bureau.screenGeometry();
    //int x = surface_bureau.width()/2 - width()/2;
    //int y = surface_bureau.height()/2 - height()/2;
    int x = surface_bureau.width()-(width()+90), y = surface_bureau.height()-(height()+100);
    move(x,y);

    //Empêcher le redimensionnement de la fenêtre
    setMinimumSize(size());
    setMaximumSize(size());

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(handle_current_tab_changed(int)));
    ui->tabWidget->setCurrentIndex(0);


    // here's the code that calculates the percentage of each type and shows it under the statistics tab
    AppData * appData = (AppData *) AppDataManager::getByKey(AppDataManager::STORAGE_DIR_KEY);

    QList<Category> * categories = CategoryManager::getAll();
    Category category1(categories->at(0)), category2(categories->at(1)),
            category3(categories->at(2)), category4(categories->at(3)), category5(categories->at(4));

    ui->progressBarLabel1->setText(category1.getName()+":");
    ui->progressBarLabel2->setText(category2.getName()+":");
    ui->progressBarLabel3->setText(category3.getName()+":");
    ui->progressBarLabel4->setText(category4.getName()+":");
    ui->progressBarLabel5->setText(category5.getName()+":");

    qDebug() << "Preferences " << endl;
    //FileManager::printAll();
    ui->progressBar1->setValue(FileManager::getSizePercentage(QFileInfo(appData->getValue()+"/"+category1.getName()), QDir(appData->getValue())));
    ui->progressBar2->setValue(FileManager::getSizePercentage(QFileInfo(appData->getValue()+"/"+category2.getName()), QDir(appData->getValue())));
    ui->progressBar3->setValue(FileManager::getSizePercentage(QFileInfo(appData->getValue()+"/"+category3.getName()), QDir(appData->getValue())));
    ui->progressBar4->setValue(FileManager::getSizePercentage(QFileInfo(appData->getValue()+"/"+category4.getName()), QDir(appData->getValue())));
    ui->progressBar5->setValue(FileManager::getSizePercentage(QFileInfo(appData->getValue()+"/"+category5.getName()), QDir(appData->getValue())));


    // And here, i fill the email field with user's email
    ui->le_email->setText(UserManager::getUser()->getEmail());

    // i fill the combobox about languages with all the languages supported
    ui->cb_language->addItem("English");

    // number of thumbnail parameter setting
    QStringList nbThumbnails;
    nbThumbnails.append("0");
    nbThumbnails.append("4");
    nbThumbnails.append("9");
    ui->cb_nbThumbnails->addItems(nbThumbnails);
    AppData * thumbSetting = AppDataManager::getByKey("thumbsNumber");
    if (thumbSetting != NULL){
        int size = nbThumbnails.size();
        for (int i(0); i < size; i++){
            if (nbThumbnails.at(i).toInt() == thumbSetting->getValue().toInt()){
                ui->cb_nbThumbnails->setCurrentIndex(i);
                break;
            }
        }
    }

    // syncing setting tab initialisation
    // Pull in data every ? minutes
    QStringList pullInOptions;
    pullInOptions.append("5 mins");
    pullInOptions.append("10 mins");
    pullInOptions.append("30 mins");
    ui->cb_pullInOption->addItems(pullInOptions);
    if (pullInAppData != NULL){
        int size = pullInOptions.size();
        for (int i =0; i < size; i++){
            int nbMinutes = pullInOptions.at(i).split(" ").at(0).toInt();
            if ((nbMinutes  * 60) == pullInAppData->getValue().toInt()){
                ui->cb_pullInOption->setCurrentIndex(i);
                break;
            }
        }
    }

    QStringList sendOutOptions;
    sendOutOptions.append("15 seconds");
    sendOutOptions.append("40 seconds");
    sendOutOptions.append("1 min");
    ui->cb_sendOutOption->addItems(sendOutOptions);

    if (sendOutAppData != NULL){
        int size = sendOutOptions.size();
        for (int i =0; i < size; i++){
            int nbSeconds = sendOutOptions.at(i).split(" ").at(0).toInt();
            if (nbSeconds == sendOutAppData->getValue().toInt()){
                ui->cb_sendOutOption->setCurrentIndex(i);
                break;
            }
        }
    }
}

PreferencesForm::~PreferencesForm()
{
    delete ui;
}

void PreferencesForm::on_bt_ok_clicked()
{
    this->hide();
}

void PreferencesForm::on_bt_cancel_clicked()
{
    this->hide();
}

void PreferencesForm::on_bt_apply_clicked()
{
    int response = QMessageBox::warning(this, "Confirmation" , "Are you ok for saving these defined settings?", QMessageBox::Yes, QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        bool recording = true;

        // i apply the modifications done on the tab with index 0
        if (ui->chkbx_launchOnStart->isChecked()){
            // the user decides to star the program on every computer boot
            QString linkPath(Parameters::STARTUP_PROG_FOLDER+"/"+Parameters::ROOT_DIR_NAME+".lnk");
            //linkPath.replace("\\","/");
            if (QFile(Parameters::SETUP_DIR+"/"+"zemini.exe").link(linkPath)){
                qDebug() << "on boot link created" << endl;
            }
            else{
                qDebug() << "Something went wrong on boot link creation" << endl;
            }
        }
        else{
            // i remove the link from the startup program folder
            if (QDir(Parameters::STARTUP_PROG_FOLDER).remove(Parameters::ROOT_DIR_NAME+".lnk")){
                qDebug() << "on boot link deleted" << endl;
            }
            else{
                qDebug() << "Something went wrong on boot link deletion" << endl;
            }

        }

        // i apply the modifications done on the tab with index 1
        AppData * thumbSetting = AppDataManager::getByKey("thumbsNumber");
        if (thumbSetting == NULL){
            if (AppDataManager::add(new AppData("thumbsNumber", ui->cb_nbThumbnails->currentText())) == NULL){
                qDebug() << "Failed to save thumbnail number setting" << endl;
                recording = false;
            }
        }
        else{
            thumbSetting->setValue(ui->cb_nbThumbnails->currentText());
            if (AppDataManager::update(thumbSetting) == NULL){
                qDebug() << "Failed to update thumbnails number setting" << endl;
                recording = false;
            }
        }

        // i apply the modifications done on the tab with index 3
        AppData * pullInAppData = AppDataManager::getByKey("pullInDataFrequency");
        AppData * sendOutAppData = AppDataManager::getByKey("sendOutDataFrequency");
        qDebug() << "test 1" << endl;
        if (pullInAppData == NULL){
            qDebug() << "test 2" << endl;
            pullInAppData = new AppData("pullInDataFrequency", QString::number(ui->cb_pullInOption->currentText().split(" ").at(0).toInt() * 60));
            if (AppDataManager::add(pullInAppData) == NULL)
                recording = false;
            qDebug() << "test 3" << endl;
        }
        else{
            pullInAppData->setValue(QString::number(ui->cb_pullInOption->currentText().split(" ").at(0).toInt() * 60));
            if (AppDataManager::update(pullInAppData) ==  NULL)
                recording = false;
        }

        if (sendOutAppData == NULL){
            sendOutAppData = new AppData("sendOutDataFrequency", QString::number(ui->cb_sendOutOption->currentText().split(" ").at(0).toInt()));
            if (AppDataManager::add(sendOutAppData) == NULL)
                recording = false;
        }
        else{
            sendOutAppData->setValue(QString::number(ui->cb_sendOutOption->currentText().split(" ").at(0).toInt()));
            if (AppDataManager::update(sendOutAppData) == NULL)
                recording = false;
        }

        // If everything went the right way then, the following message is displayed
        if (recording)
            QMessageBox::information(0, "Zemini Info", "Settings successfully saved!");
        else
            QMessageBox::warning(0, "Zemini Info", "Something went wrong when saving the defined settings. Please, try later!");
    }

    this->hide();
}

void PreferencesForm::on_bt_otherLocation_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this);
    if (!folderName.isEmpty())
    {
        ui->le_otherLocation->setText(folderName);
    }
    //QDesktopServices::openUrl(QUrl("file:///"+fileName, QUrl::TolerantMode));
}

void PreferencesForm::on_chkbx_launchOnStart_clicked(bool checked)
{

}

void PreferencesForm::handle_current_tab_changed(int position)
{
    switch (position)
    {
    case 0: // general settings
        ui->bt_ok->setVisible(true);
        ui->bt_apply->setVisible(true);
        break;

    case 1: // account tab
        ui->bt_ok->setVisible(true);
        ui->bt_apply->setVisible(true);
        break;

    case 2: // statistics tab
        //i hide some buttons
        ui->bt_ok->setVisible(false);
        ui->bt_apply->setVisible(false);
        break;

    case 3: // sync tab
        //i hide some buttons
        ui->bt_ok->setVisible(true);
        ui->bt_apply->setVisible(true);
        break;
    }
}
