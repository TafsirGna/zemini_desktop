#include "Controllers/MainController.h"

/*** Default constructor    ***/
MainController::MainController()
{
    serviceContainer = new ServiceContainer();
    registerForm = NULL;
    logInForm = NULL;
    preferencesForm = NULL;

    // Variable that determine if it's the first launch
    firstLaunch = false;
    pausedProcess = false;
    screenShotTimer = new QTimer(this);
    trayInfoTimer = NULL;
    trayTitle = "Zemini Info";

    // Initialization of all the forms
    if (!installationCompleted()){
        registerForm = new RegisterForm(0, this->serviceContainer);
        logInForm = new LogInForm(0, this->serviceContainer);

        QWidget::connect(registerForm, SIGNAL(logInLinkActivated()),this, SLOT(showLogInForm()));
        QWidget::connect(logInForm, SIGNAL(signUpLinkActivated()), this, SLOT(showRegisterForm()));
        QWidget::connect(logInForm, SIGNAL(closeAppSignal()), this, SLOT(close()));
        QWidget::connect(logInForm, SIGNAL(userLoggedIn()), this, SLOT(start()));
        QWidget::connect(registerForm, SIGNAL(userRegistered()), this, SLOT(start()));
    }

    aboutForm = new AboutForm(0);
    uploadingForm = new UploadingForm(0, this->serviceContainer);
    trayIcon = new QSystemTrayIcon();

    //Setting connections with the child threads
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirUpdated(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileUpdate(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(rootFolderOnWatching()), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(startBackingUp()));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileUpdated(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileUpdate(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileDeleted(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileDeletion(QFileInfo)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(requestReplyReceived(QMap<QString,QString>,QList<DbEntity*>*)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(onRequestReplyReceived(QMap<QString,QString>,QList<DbEntity*>*)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirDeleted(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(saveFileDeletion(QFileInfo)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(filesToSend(QString, QList<DbEntity *>*)), ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SLOT(send(QString, QList<DbEntity*>*)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(thumbBackedUp(File *)), ((DirectoryService*)this->serviceContainer->getService(ZeminiService::FileSystem)), SLOT(onThumbBackedUp(File *)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(Parameters::DB_FILE), SIGNAL(fileSaved(File *)), ((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SLOT(watchFile(File *)));
    QWidget::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(manageActivation(QSystemTrayIcon::ActivationReason)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(storeInDb(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(save(QFileInfo)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(readyToBackUp()), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(startBackingUp()));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(firstBackUpDone()), this, SLOT(onFirstBackUpDone()));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(requestFailed(int)), this, SLOT(onRequestFailed(int)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(connectionError(int)), this, SLOT(onConnectionError(int)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(userEnabled(bool)), this, SLOT(onUserEnabled(bool)));
    QWidget::connect(screenShotTimer, SIGNAL(timeout()), this, SLOT(takeScreenShot()));
    QWidget::connect(uploadingForm, SIGNAL(processPaused()), this, SLOT(onProcessPaused()));
    QWidget::connect(uploadingForm, SIGNAL(processCancelled()), this, SLOT(onProcessCancelled()));
    QWidget::connect(uploadingForm, SIGNAL(processResumed()), this, SLOT(onProcessResumed()));
    QWidget::connect(uploadingForm, SIGNAL(processRestarted()), this, SLOT(onProcessRestarted()));
    //QWidget::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showDirectory()));

    // setting the contextual menu
    //building the context menu
    QAction * openFolder = new QAction(QIcon(),tr("Open Zemini folder"),this);
    QWidget::connect(openFolder, SIGNAL(triggered()),this, SLOT(showDirectory()));
    QAction * openPreferences = new QAction(QIcon(),tr("Preferences..."),this);
    QWidget::connect(openPreferences, SIGNAL(triggered()), this , SLOT(showPreferences()));
    QAction * quitZemini = new QAction(QIcon(),tr("Quit Zemini"),this);
    QWidget::connect(quitZemini, SIGNAL(triggered()), this , SLOT(stop()));
    QAction * openZeminiweb = new QAction(QIcon(),"Go to Zemini Web",this);
    QWidget::connect(openZeminiweb, SIGNAL(triggered()), this , SLOT(showZeminiWebSite()));
    //QAction * recordScreen = new QAction(QIcon(),tr("Start recording"),this);
    //QWidget::connect(recordScreen, SIGNAL(triggered()), this , SLOT(recordScreen()));
    QAction * watchVideo = new QAction(QIcon(),tr("Watch video"),this);
    QWidget::connect(watchVideo, SIGNAL(triggered()), this , SLOT(openVideoForm()));
    QAction * aboutZemini = new QAction(QIcon(),tr("About"),this);
    QWidget::connect(aboutZemini, SIGNAL(triggered()), this , SLOT(showAboutForm()));
    QAction * uploadingProcess = new QAction(QIcon(),tr("Syncing files ... (1%)"),this);
    uploadingProcess->setEnabled(false);
    QWidget::connect(uploadingProcess, SIGNAL(triggered()), this , SLOT(showUploadingForm()));

    QMenu * contextMenu = new QMenu();
    contextMenu->addAction(openFolder);
    contextMenu->addAction(watchVideo);
    //contextMenu->addAction(recordScreen);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(openPreferences);
    contextMenu->addAction(openZeminiweb);
    contextMenu->addAction(uploadingProcess);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(aboutZemini);
    contextMenu->addAction(quitZemini);

    //Setting the context menu to the system tray entrie
    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(Parameters::iconLocation));
    trayIcon->setToolTip("Zemini "+Parameters::APP_VERSION +" \nConnecting...");

    uploadingForm->setTrayIcon(trayIcon);
}

void MainController::start()
{
    if (!installationCompleted()){
        //complete zemini installation
        firstLaunch = true;
        completeInstallation();
    }
    else{
        // have to do that before continuing
        if (registerForm != NULL && logInForm != NULL){
            delete(registerForm);
            delete(logInForm);
        }

        LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
        UserManager * userManager = (UserManager *) localDbService->getManager(Parameters::DB_USER);
        Parameters::NB_THUMBS_PER_FILE = AppDataManager::getByKey("thumbsNumber")->getValue().toInt();

        User * user = userManager->getUser();
        // if the user account is not activated
        qDebug() << "Starting 2" << endl;
        ((NetworkService *) this->getService(ZeminiService::Network))->setUser(user);
        if (!user->isActivated()){
            qDebug() << "Starting 3" << endl;
            checkAccountConfirmation();
        }
        else{
            if (firstLaunch){
                trayMessage = "Welcome to Zemini, Enjoy zemini's features";
                showTrayInfo();
            }

            DirectoryService * directoryService = (DirectoryService *)this->getService(ZeminiService::FileSystem);
            // noticing to the user that we're starting watching the root folder
            trayIcon->show();
            directoryService->start();
        }
    }
}

/***    this opens the windows' explorer on the zemini folder   ***/
void MainController::showDirectory()
{
    QString dirPath = DriveManager::getDefaultDrive()->getAbsolutepath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath.replace("\\","/")));
}

void MainController::showAboutForm()
{
    aboutForm->show();
}


/***    this manages events related to the activation of the system tray entrie ***/
void MainController::manageActivation(QSystemTrayIcon::ActivationReason reason)
{
    QPoint point(QCursor::pos().x(), QCursor::pos().y()-125);
    trayIcon->contextMenu()->move(point);
    trayIcon->contextMenu()->show();
}

/***    this shows up the preference form on the screen ***/
void MainController::showPreferences()
{
    if (preferencesForm == NULL)
    {
        preferencesForm = new PreferencesForm(0, this->serviceContainer);
        preferencesForm->show();
    }
    else
        preferencesForm->setVisible(true);

    // Connecting signals to slots
    //QWidget::connect(preferencesForm, SIGNAL(setupDirectory(QStringList)),this, SLOT(addFileSystemWatcher(QStringList)));
}

/***    this launches a web navigator to the zemini web site url    ***/
void MainController::showZeminiWebSite()
{
    QDesktopServices::openUrl(QUrl(Parameters::WEB_SITE));
}

void MainController::openVideoForm()
{

}

void MainController::showUploadingForm()
{
    uploadingForm->show();
}

/***            Once, the user installs the sofware, at the first launch he must complete the pocedure by supplying some informations           ***/
void MainController::completeInstallation()
{    
    // Two ways to check the completeness of the installation
    // First, the database status
    if (((LocalDBService * ) this->getService(ZeminiService::LocalDatabase))->isDbEmpty()){
        registerForm->show();
        return;
    }

    //deleting the stuff , i don't need anymore before continuing the process
    registerForm->close();
    delete(registerForm);

    // Second, the zemini directory status
    Drive * drive = DriveManager::getDefaultDrive();

    if (drive == NULL){
        logInForm->show();
        logInForm->waiting();
        if (!DirectoryService::setUserFolder(logInForm)){
            //qDebug() << "end app" << endl;
            qApp->quit();
        }
        else{
            start();
        }
    }
}

void MainController::showLogInForm()
{
    registerForm->hide();
    logInForm->show();
}


void MainController::showRegisterForm()
{
    logInForm->hide();
    registerForm->setVisible(true);
}

bool MainController::installationCompleted()
{
    // Two ways to check the completeness of the installation
    // First, the database status
    if (((LocalDBService * ) this->getService(ZeminiService::LocalDatabase))->isDbEmpty())
        return false;

    // Second, the zemini directory status
    Drive * drive = DriveManager::getDefaultDrive();
    if (drive == NULL)
        return false;
    return true;
}

void MainController::stop()
{
    // before closing the app, let's save some updates into the db
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    localDbService->update(new AppData(AppData::LAST_EXIT, QDateTime::currentDateTime().toString(Parameters::timeFormat)));

    close();
}

ZeminiService * MainController::getService(QString service)
{
    return this->serviceContainer->getService(service);
}

bool MainController::checkInitialDbData()
{
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    CategoryManager * categoryManager = (CategoryManager *) localDbService->getManager(Parameters::DB_CATEGORY);
    if (categoryManager->isEmpty())
        return false;
    return true;
}


void MainController::setArgs(AbstractController * controller)
{
    controller->setServiceContainer(this->serviceContainer);
}

/**
 * @brief MainController::recordScreen
 */
void MainController::recordScreen()
{
    // we take a first a first screenshot that's gonna be usefull for defining the video features
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;
    QPixmap pix = screen->grabWindow(0);
    cv::Mat frame = (cv::Mat) QImage2Mat(pix.toImage());

    // then, i go on defining the videowriter
    screenRecordWriter = new cv::VideoWriter();
    int codec = CV_FOURCC('M', 'J', 'P', 'G');
    screenRecordWriter->open(cv::String("test.avi"), codec, 30, frame.size(), true);
    if (!screenRecordWriter->isOpened()) {
        qDebug() << "Failed to open the output file " << endl;
        return;
    }
    qDebug() << "Succeed in opening the output file " << endl;
    this->NB_FRAMES_COUNTER = ((Parameters::MAX_SCREEN_REC_TIME * Parameters::NB_FRAMES_PER_SEC) / 1000);
    screenShotTimer->start(1000/Parameters::NB_FRAMES_PER_SEC);
}

/**
 * @brief MainController::takeScreenShot
 * @return
 */
void MainController::takeScreenShot()
{
    if (NB_FRAMES_COUNTER == 0){
        screenShotTimer->stop();
        QString title = "Zemini Info";
        QString message = "File saved";
        trayIcon->showMessage(title, message);
        delete(screenRecordWriter);
        return;
    }
    // First, i start taking screenshot
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    QPixmap pix = screen->grabWindow(0);
    // i save the pixmap object in the file system
    //if (!pix.save(QDir().homePath()+"/test.png"))
    //    qDebug() << "Failed to save the screenshot!" << endl;

    cv::Mat frame = (cv::Mat) QImage2Mat(pix.toImage());

    screenRecordWriter->write(frame);
    NB_FRAMES_COUNTER--;
    //qDebug() << NB_FRAMES_COUNTER << endl;

    if (!cv::imwrite("test.png", frame))
        qDebug() << "Failed to save the screenshot!" << endl;
}

/**
 * @brief MainController::onFirstBackUpDone
 */
void MainController::onFirstBackUpDone()
{
    QString title = "Zemini Info";
    QString message = "First backup done.";
    qDebug() << "First backup done." << endl;
    trayIcon->showMessage(title, message);
}

void MainController::onRequestFailed(int requestCode)
{
    if (requestCode == Parameters::CODE_ACCOUNT_CHECKING){
        //The way to handle this error is the same with the one to handle the connection error
        onConnectionError(Parameters::CODE_ACCOUNT_CHECKING);
    }
}

void MainController::onConnectionError(int code)
{
    //qDebug() << "test" << endl;
    if (code == Parameters::CODE_ACCOUNT_CHECKING){
        DirectoryService * directoryService = (DirectoryService *)this->getService(ZeminiService::FileSystem);
        if (!directoryService->isActive()){
            QString title = "Zemini Info";
            QString message = "It seems you are not connected to Internet!";
            trayIcon->showMessage(title, message);

            // noticing to the user that we're starting watching the root folder
            trayIcon->show();
            directoryService->start();
        }
    }
}

void MainController::onUserEnabled(bool enabled)
{
    if (!enabled){
        QMessageBox::information(this, "Zemini Info", "Before fully enjoy Zemini, we invite you to check the email we've sent to you. Thanks!", QMessageBox::Ok);

        // show periodically a message to the user for noticing him that his account was not confirmed
        trayInfoTimer = new QTimer(this);
        trayMessage = "You've not confirmed your email account. Please, check your email!";
        QWidget::connect(trayInfoTimer, SIGNAL(timeout()), this, SLOT(showTrayInfo()));
        trayInfoTimer->start();
    }

    ((NetworkService *)this->getService(ZeminiService::Network))->setUser(UserManager::getUser());
    trayIcon->show();
    ((DirectoryService *)this->getService(ZeminiService::FileSystem))->start();
}

QImage  MainController::cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap MainController::cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

cv::Mat3b MainController::QImage2Mat(const QImage &src) {
    unsigned int height = src.height();
    unsigned int width = src.width();

    cv::Mat3b dest(height, width);
    for (unsigned int y = 0; y < height; ++y) {
        cv::Vec3b *destrow = dest[y];
        for (unsigned int x = 0; x < width; ++x) {
            QRgb pxl = src.pixel(x, y);
            destrow[x] = cv::Vec3b(qBlue(pxl), qGreen(pxl), qRed(pxl));
        }
    }
    return dest;
}

void MainController::checkAccountConfirmation()
{
    ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network))->checkUserAccount();
}

void MainController::onProcessPaused()
{
    pausedProcess = true;
}

void MainController::onProcessResumed()
{
    pausedProcess = false;
}

void MainController::onProcessCancelled()
{

}

void MainController::onProcessRestarted()
{

}

void MainController::showTrayInfo()
{
    trayIcon->showMessage(trayTitle, trayMessage);
}

