#include "UploadingForm.h"
#include "ui_UploadingForm.h"

UploadingForm::UploadingForm(QWidget *parent, ServiceContainer * serviceContainer) :
    QWidget(parent),
    ui(new Ui::UploadingForm)
{
    ui->setupUi(this);

    this->serviceContainer = serviceContainer;
    currentStep = 1;
    uploadingSteps.insert("1", "Saving and tracking files ...");
    uploadingSteps.insert("2", "Uploading files' data ...");

    paused = false;
    cancelled = false;
    uploadingFiles = false;

    QPixmap pixmap(Parameters::iconLocation);
    // make a circle image
    QBitmap mask(pixmap.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), mask.width()/2, mask.height()/2);
    pixmap.setMask(mask);
    ui->app_logo->setPixmap(pixmap);
    ui->app_logo->setScaledContents(true);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);

    ui->pt_detail->setVisible(false);

    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowType::Window);

    //Connecting signals to slots
    QWidget::connect(this, SIGNAL(processPaused()), ((NetworkService *) serviceContainer->getService(ZeminiService::Network)), SLOT(onProcessPaused()));
    QWidget::connect(this, SIGNAL(processCancelled()), ((NetworkService *) serviceContainer->getService(ZeminiService::Network)), SLOT(onProcessCancelled()));
    QWidget::connect(this, SIGNAL(processPaused()), ((DirectoryService *) serviceContainer->getService(ZeminiService::FileSystem)), SLOT(onProcessPaused()));
    QWidget::connect(this, SIGNAL(processCancelled()), ((DirectoryService *) serviceContainer->getService(ZeminiService::FileSystem)), SLOT(onProcessCancelled()));

    QWidget::connect(this, SIGNAL(processResumed()), ((NetworkService *) serviceContainer->getService(ZeminiService::Network)), SLOT(onProcessResumed()));
    QWidget::connect(this, SIGNAL(processRestarted()), ((NetworkService *) serviceContainer->getService(ZeminiService::Network)), SLOT(onProcessRestarted()));
    QWidget::connect(this, SIGNAL(processResumed()), ((DirectoryService *) serviceContainer->getService(ZeminiService::FileSystem)), SLOT(onProcessResumed()));
    QWidget::connect(this, SIGNAL(processRestarted()), ((DirectoryService *) serviceContainer->getService(ZeminiService::FileSystem)), SLOT(onProcessRestarted()));

    QWidget::connect(((LocalDBService *) serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(filesToSend(QString, QList<DbEntity *>*)), this, SLOT(onStartSendingFiles(QString, QList<DbEntity *>*)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(Parameters::DB_FILE), SIGNAL(fileUpdated(File *)), this, SLOT(onFileChange(File*)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(Parameters::DB_FILE), SIGNAL(fileSaved(File *)), this, SLOT(onFileChange(File*)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(fileBackedUp(File*)), this, SLOT(onFileBackedUp(File*)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(startWatchingRootDir()), this, SLOT(onStartWatchingRootDir()));
}

UploadingForm::~UploadingForm()
{
    delete ui;
}

void UploadingForm::setTrayIcon(QSystemTrayIcon * sysTrayIcon)
{
    trayIcon = sysTrayIcon;
}

void UploadingForm::on_bt_background_clicked()
{
    this->hide();
}

void UploadingForm::on_bt_detail_clicked()
{
    if (ui->pt_detail->isVisible()){
        ui->pt_detail->hide();
        ui->bt_detail->setText("Show details");
    }
    else{
        ui->pt_detail->show();
        ui->bt_detail->setText("Hide details");
    }
}

void UploadingForm::onFileChange(File * file)
{
    if (uploadingFiles){
        //qDebug() << "path is : " << file->getAbsolutePath() <<endl;
        if (QFileInfo(file->getAbsolutePath()).isFile()){
            //qDebug() << "totalSize 1: " << totalSize << filesDoneSize << file->getSize() <<endl;
            filesDoneSize += Functions::fromOctect2Ko(file->getSize());
            //qDebug() << "totalSize 2 : " << totalSize << filesDoneSize << file->getSize() <<endl;
            double percentage =  ((filesDoneSize * 100 )/ totalSize);
            ui->progressBar->setValue(percentage);

            QMenu * contextMenu = trayIcon->contextMenu();
            QAction *menuAction = contextMenu->actions().at(4);
            menuAction->setText("Saving files ... "+QString::number(percentage)+"%");
        }
        ui->pt_detail->appendPlainText(file->getAbsolutePath() + " ... done.");
    }
    else{

    }
}

void UploadingForm::onStartWatchingRootDir()
{
    uploadingFiles = true;
    currentStep = 1;
    QMenu * contextMenu = trayIcon->contextMenu();
    QAction *menuAction = contextMenu->actions().at(4);
    menuAction->setText("Preparing ...");
    ui->lb_step->setText("Preparing ...");
    ui->pt_detail->clear();
    totalSize = Functions::fromOctect2Ko(FileManager::getSizeOnDb(QFileInfo(Parameters::ROOT_DIR_PATH)));

    ui->lb_step->setText("Step "+QString::number(currentStep)+"/"+QString::number(uploadingSteps.size())+" : "+uploadingSteps[QString::number(currentStep)]);
    ui->pt_detail->appendPlainText("*** Step "+QString::number(currentStep)+"/"+QString::number(uploadingSteps.size())+" : "+uploadingSteps[QString::number(currentStep)]);
    filesDoneSize = 0;
    menuAction->setEnabled(true); //enabling the context menu action
    menuAction->setText("Saving files ... "+QString::number(filesDoneSize)+"%");
    ui->progressBar->setValue(filesDoneSize);
}

void UploadingForm::onStartSendingFiles(QString tableName, QList<DbEntity *> * entities)
{
    currentStep = 2;
    if (tableName == Parameters::DB_FILE){

        filesDoneSize = 0;
        totalSize = 0;
        int list_size = entities->size();
        for (int i (0); i < list_size; i++){
            File * file = (File *) entities->at(i);
            totalSize += file->getSize();
        }

        QMenu * contextMenu = trayIcon->contextMenu();
        QAction *menuAction = contextMenu->actions().at(4);
        menuAction->setText("Uploading ... "+QString::number(filesDoneSize)+"%");

        ui->lb_step->setText("Step "+QString::number(currentStep)+"/"+QString::number(uploadingSteps.size())+" "+uploadingSteps[QString::number(currentStep)]);
        ui->pt_detail->appendPlainText("*** Step "+QString::number(currentStep)+"/"+QString::number(uploadingSteps.size())+" : "+uploadingSteps[QString::number(currentStep)]);
        ui->progressBar->setValue(filesDoneSize);

        User * user = UserManager::getUser();
        if (!user->isActivated()){
            ui->pt_detail->appendHtml("<b style=\"color:#ff0000;\">You are not activated yet!</b>");
            return;
        }


    }
}

void UploadingForm::on_bt_pause_clicked()
{
    if (!paused){
        emit processPaused();
        ui->lb_step->setText("<span style = \"color:green\">Process paused</span>");
        ui->pt_detail->appendHtml("<b style = \"color:green\">Process paused</b>");
        ui->bt_pause->setText("Resume");
        paused = true;
    }
    else{
        emit processResumed();
        ui->lb_step->setText("Step "+QString::number(currentStep)+"/"+QString::number(uploadingSteps.size())+" : "+uploadingSteps[QString::number(currentStep)]);
        ui->pt_detail->appendHtml("<b style = \"color:green\">Process resumed</b>");
        ui->bt_pause->setText("Pause");
        paused = false;
    }
}

void UploadingForm::on_bt_cancel_clicked()
{
    if (!cancelled){
        emit processCancelled();
        ui->lb_step->setText("<span style = \"color:green\">Process cancelled</span>");
        ui->progressBar->setValue(0);
        ui->pt_detail->appendHtml("<b style = \"color:green\">Process cancelled</b>");
        ui->bt_cancel->setText("Restart");
        cancelled = true;
    }
    else{
        emit processRestarted();
        ui->lb_step->setText("<span style = \"color:green\">Process cancelled</span>");
        ui->progressBar->setValue(0);
        ui->pt_detail->appendHtml("<b style = \"color:green\">Process cancelled</b>");
        ui->bt_cancel->setText("Cancel");
        cancelled = false;
    }
}

void UploadingForm::onFileBackedUp(File * file)
{
    if (QFileInfo(file->getAbsolutePath()).isFile()){
        //qDebug() << "totalSize 1: " << totalSize << filesDoneSize << file->getSize() <<endl;
        filesDoneSize += Functions::fromOctect2Ko(file->getSize());
        //qDebug() << "totalSize 2 : " << totalSize << filesDoneSize << file->getSize() <<endl;
        double percentage =  ((filesDoneSize * 100 )/ totalSize);
        ui->progressBar->setValue(percentage);

        QMenu * contextMenu = trayIcon->contextMenu();
        QAction *menuAction = contextMenu->actions().at(4);
        menuAction->setText("Uploading ... "+QString::number(percentage)+"%");
    }
    ui->pt_detail->appendPlainText(file->getAbsolutePath() + " ... done.");
}
