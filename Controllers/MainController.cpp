#include "Controllers/MainController.h"

/*** Default constructor    ***/
MainController::MainController()
{
    serviceContainer = new ServiceContainer();

    // Variable that determine if it's the first launch
    firstLaunch = false;
    screenShotTimer = new QTimer(this);

    // Initialization of all the forms
    registerForm = new RegisterForm(0, this->serviceContainer);
    logInForm = new LogInForm(0, this->serviceContainer);
    trayIcon = new QSystemTrayIcon();

    //Setting connections with the child threads
    QWidget::connect(registerForm,SIGNAL(logInLinkActivated()),this,SLOT(showLogInForm()));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirUpdated(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileUpdate(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileUpdated(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileUpdate(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileDeleted(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileDeletion(QFileInfo)));
    //QWidget::connect(logInForm, SIGNAL(onStart(AbstractController * )), this, SLOT(setArgs(AbstractController *)));
    QWidget::connect(logInForm, SIGNAL(signUpLinkActivated()), this, SLOT(showRegisterForm()));
    QWidget::connect(logInForm, SIGNAL(userToSave(User*)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(save(User *)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(initDataGot(QList<Category>*)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(initDb(QList<Category> *)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(userLoggedIn()), this, SLOT(setUserFolder()));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirDeleted(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(saveFileDeletion(QFileInfo)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(filesToSend(QList<File *>*)), ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SLOT(sendFiles(QList<File*>*)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(LocalDBService::FILE), SIGNAL(fileSaved(QFileInfo)), ((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SLOT(watchFile(QFileInfo)));
    //QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(LocalDBService::FILE), SIGNAL(fileSaved(File*)), ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SLOT(sendFile(File*)));
    QWidget::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(manageActivation(QSystemTrayIcon::ActivationReason)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(storeInDb(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(save(QFileInfo)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(readyToBackUp()), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(startBackingUp()));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(fileSaved(int)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(markFileSaved(int)));
    QWidget::connect(screenShotTimer, SIGNAL(timeout()), this, SLOT(takeScreenShot()));
    //QWidget::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showDirectory()));
}

void MainController::start()
{
    if (!installationCompleted()){
        //complete zemini installation
        firstLaunch = true;

        // Sending a request to get all initial data
        ((NetworkService *) this->getService(ZeminiService::Network))->getInitialDbData();

        completeInstallation();
    }
    else{
        displayTrayIcon();
        // if the user account is not activated
        LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
        UserManager * userManager = (UserManager *) localDbService->getManager(LocalDBService::USER);

        User * user = userManager->getUser();
        if (!user->isActivated()){
            QString title = "Zemini Info";
            QString message = "You've not activated your account yet! We invite you to do so.";
            trayIcon->showMessage(title, message);
        }
        else{
            if (firstLaunch){
                QString title = "Zemini Info";
                QString message = "Welcome to Zemini, Enjoy zemini's features";
                trayIcon->showMessage(title, message);
            }
        }
        ((NetworkService *) this->getService(ZeminiService::Network))->setUser(user);
        ((NetworkService *) this->getService(ZeminiService::Network))->getFreshDbData();
        DirectoryService * directoryService = (DirectoryService *)this->getService(ZeminiService::FileSystem);
        // noticing to the user that we're starting watching the root folder
        directoryService->start();
    }
}

/***    this opens the windows' explorer on the zemini folder   ***/
void MainController::showDirectory()
{
    QString dirPath = Parameters::STORE_DIR;
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath.replace("\\","/")));
}


/***    this manages events related to the activation of the system tray entrie ***/
void MainController::manageActivation(QSystemTrayIcon::ActivationReason reason)
{
    trayIcon->contextMenu()->move(QCursor::pos());
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
    QWidget::connect(preferencesForm, SIGNAL(setupDirectory(QStringList)),this, SLOT(addFileSystemWatcher(QStringList)));
}

/***    this launches a web navigator to the zemini web site url    ***/
void MainController::showZeminiWebSite()
{
    QDesktopServices::openUrl(QUrl(Parameters::WEB_SITE));
}

/***            Once, the user installs the sofware, at the first launch he must complete the pocedure by supplying some informations           ***/
void MainController::completeInstallation()
{
    registerForm->show();
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
    QString dirPath = Parameters::STORE_DIR;
    if (!QDir(dirPath.replace("\\","/")).exists())
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

/**
 * @brief MainController::displayTrayIcon
 * @return
 */
bool MainController::displayTrayIcon()
{
    //building the context menu
    QAction * openFolder = new QAction(QIcon(),tr("Open Zemini folder"),this);
    QWidget::connect(openFolder, SIGNAL(triggered()),this, SLOT(showDirectory()));
    QAction * openPreferences = new QAction(QIcon(),tr("Preferences..."),this);
    QWidget::connect(openPreferences, SIGNAL(triggered()), this , SLOT(showPreferences()));
    QAction * quitZemini = new QAction(QIcon(),tr("Quit Zemini"),this);
    QWidget::connect(quitZemini, SIGNAL(triggered()), this , SLOT(stop()));
    QAction * openZeminiweb = new QAction(QIcon(),"Go to Zemini Web",this);
    QWidget::connect(openZeminiweb, SIGNAL(triggered()), this , SLOT(showZeminiWebSite()));
    QAction * recordScreen = new QAction(QIcon(),tr("Start recording"),this);
    QWidget::connect(recordScreen, SIGNAL(triggered()), this , SLOT(recordScreen()));

    QMenu * contextMenu = new QMenu();
    contextMenu->addAction(openFolder);
    contextMenu->addAction(recordScreen);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(openPreferences);
    contextMenu->addAction(openZeminiweb);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(quitZemini);

    //Setting the context menu to the system tray entrie
    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(Parameters::refreshingIconLocation));
    trayIcon->setToolTip("Zemini "+Parameters::APP_VERSION +" \nStarting...");
    trayIcon->show();
}

ZeminiService * MainController::getService(QString service)
{
    return this->serviceContainer->getService(service);
}

bool MainController::checkInitialDbData()
{
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    CategoryManager * categoryManager = (CategoryManager *) localDbService->getManager(LocalDBService::CATEGORY);
    if (categoryManager->isEmpty())
        return false;
    return true;
}

/**
 * @brief MainController::setUserFolder
 * @return
 */
bool MainController::setUserFolder()
{
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    CategoryManager * categoryManager = (CategoryManager *) localDbService->getManager(LocalDBService::CATEGORY);

    // making the directories following the categories
    QList<Category> * categories = categoryManager->getAllCategories();
    ((DirectoryService * ) this->getService(ZeminiService::FileSystem))->makeInitDirectories(categories);
    this->start();
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
    screenRecordWriter = new cv::VideoWriter();
    screenRecordWriter->open(cv::String("test.avi"), -1, 30, cv::Size(1024,768), true);
    if (!screenRecordWriter->isOpened()) {
        qDebug() << "Failed to open the output file " << endl;
        return;
    }
    //qDebug() << "succeed in opening the output file " << endl;
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

    (*screenRecordWriter) << frame;

    NB_FRAMES_COUNTER--;
    /*
    if (!cv::imwrite("test.png", frame))
        qDebug() << "Failed to save the screenshot!" << endl;
    else
        qDebug() << "Succeed in saving the screenshot!" << endl;
    */
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

