#include "Controllers/RegisterForm.h"
#include "ui_RegisterForm.h"

RegisterForm::RegisterForm(QWidget *parent, ServiceContainer *serviceContainer) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);

    // Setting the regExpr for email lineEdit
    emailValidator = new QRegExpValidator(*(Parameters::emailRegExpr), this);

    wSpinnerWidget = new WaitingSpinnerWidget(this, Qt::ApplicationModal, true);
    wSpinnerWidget->start();

    // display the user default image on the top of the form
    QPixmap pixmap(Parameters::userIcon);

    // make a circle image
    QBitmap mask(pixmap.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), mask.width()/2, mask.height()/2);
    pixmap.setMask(mask);

    ui->lb_user_picture->setPixmap(pixmap);
    ui->lb_user_picture->setScaledContents(true);

    //on the show of the form, the field of name get focused
    ui->le_username->setFocus();
    ui->lb_error_text->setText("");
    ui->lb_error_text->setStyleSheet("color: red;");
    ui->le_password->setEchoMode(QLineEdit::Password);

    // This commands put the form on the middle of the screen
    QDesktopWidget bureau;
    QRect surface_bureau = bureau.screenGeometry();
    int x = surface_bureau.width()/2 - width()/2;
    int y = surface_bureau.height()/2 - height()/2;
    move(x,y);

    // Prevent the resizing of the window
    setMinimumSize(size());
    setMaximumSize(size());

    this->serviceContainer = serviceContainer;
    networkService = (NetworkService *) serviceContainer->getService(ZeminiService::Network);
    localDbService = (LocalDBService *) serviceContainer->getService(ZeminiService::LocalDatabase);
    QWidget::connect(localDbService, SIGNAL(dbInitialized()), this, SLOT(onDbInitialized()));
    QWidget::connect(networkService, SIGNAL(connectionError(int)), this, SLOT(onConnectionError(int)));
    QWidget::connect(networkService, SIGNAL(requestFailed(int)), this, SLOT(onRequestFailed(int)));
    QWidget::connect(localDbService, SIGNAL(userSaved(int)), this, SLOT(onUserSaved(int)));
    networkService->getInitialDbData();

    initDbTimer = new QTimer(this);
    initDbTimer->setInterval(Parameters::CHECK_CON_TIME_OUT);
    QWidget::connect(initDbTimer, SIGNAL(timeout()), this, SLOT(resumeDbInit()));
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::onUserNotRegistered()
{
    ui->lb_error_text->setText("An error occured. Restart the app later");
    showErrorSignes(ui->lb_username, ui->le_username);
    showErrorSignes(ui->lb_mail, ui->le_mail);
    showErrorSignes(ui->lb_password, ui->le_password);
}

void RegisterForm::on_bt_next_clicked()
{
    //i check if all textedits have been filled
    if (ui->le_username->text().isEmpty())
    {
        ui->lb_error_text->setText("Empty username field");
        showErrorSignes(ui->lb_username, ui->le_username);
        ui->le_username->setFocus();
        return;
    }

    if (!isEmailValid())
        return;

    if (!isPasswordValid())
        return;

    // building a user object with all informations gathered
    user = new User("", "", ui->le_mail->text(), ui->le_username->text(), ui->le_password->text());

    // sending a post request to put the user in the remote db
    QList<DbEntity*> * data = new QList<DbEntity*>();
    data->append(user);
    wSpinnerWidget->start();
    networkService->send(Parameters::DB_USER, data);
}

void RegisterForm::on_le_username_textChanged(const QString &arg1)
{
    // i remove every sign of error previously set
    if (ui->lb_error_text->isVisible() || ui->lb_error_text->text() == "Error connection"){
        return;
    }

    removeErrorSignes(ui->lb_username, ui->le_username);
}

void RegisterForm::on_le_mail_textChanged(const QString &arg1)
{
    if (ui->lb_error_text->isVisible() || ui->lb_error_text->text() == "Error connection"){
        return;
    }
    removeErrorSignes(ui->lb_mail, ui->le_mail);
}

void RegisterForm::on_le_password_textChanged(const QString &arg1)
{
    if (ui->lb_error_text->isVisible() || ui->lb_error_text->text() == "Error connection"){
        return;
    }
    removeErrorSignes(ui->lb_password, ui->le_password);
}

void RegisterForm::showErrorSignes(QLabel * label, QLineEdit * lineEdit)
{
    ui->lb_error_text->setVisible(true);
    label->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
    lineEdit->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
}

void RegisterForm::removeErrorSignes(QLabel * label, QLineEdit * lineEdit)
{
    if (ui->lb_error_text->isVisible()){
        ui->lb_error_text->setVisible(false);
        label->setStyleSheet("color: black; font-family: Palatino Linotype; font-size: 13px;");
        lineEdit->setStyleSheet("font-family: Palatino Linotype; font-size: 13px;");
    }
}

void RegisterForm::on_lb_sign_in_linkActivated(const QString &link)
{
    emit logInLinkActivated();
}

bool RegisterForm::isPasswordValid()
{
    QString password = ui->le_password->text();

    if (password.isEmpty()){
        ui->lb_error_text->setText("Empty password field");
        showErrorSignes(ui->lb_password, ui->le_password);
        ui->le_password->setFocus();
        return false;
    }

    if (password.length()<=4){
        ui->lb_error_text->setText("Password too short");
        showErrorSignes(ui->lb_password, ui->le_password);
        ui->le_password->setFocus();
        return false;
    }

    return true;
}

bool RegisterForm::isEmailValid()
{
    QString user_mail = ui->le_mail->text();

    if (user_mail.isEmpty()){
        ui->lb_error_text->setText("Empty email field");
        showErrorSignes(ui->lb_mail, ui->le_mail);
        ui->le_mail->setFocus();
        return false;
    }

    // i check if the format of the mail entered is right
    int pos = 0;
    if (this->emailValidator->validate(user_mail, pos) != QValidator::Acceptable){
        ui->lb_error_text->setText("Wrong email format");
        showErrorSignes(ui->lb_mail, ui->le_mail);
        ui->le_mail->setFocus();
        return false;
    }
    return true;
}

void RegisterForm::resumeDbInit()
{
    initDbTimer->stop();
    networkService->sendNextRequest();
}

void RegisterForm::onConnectionError(int requestCode)
{
    if (requestCode == Parameters::CODE_DB_INIT){
        // dismiss the loading spinner
        wSpinnerWidget->stop();

        if (ui->lb_error_text->isVisible()){
            ui->lb_error_text->setText("Error connection");
            ui->lb_error_text->setVisible(true);

            ui->lb_username->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
            ui->le_username->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

            ui->lb_mail->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
            ui->le_mail->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

            ui->lb_password->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
            ui->le_password->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
        }

        // after reporting the connection error, i start a timer that gonna periodically check the connection status
        initDbTimer->start();
    }
}

void RegisterForm::onUserSaved(int code)
{
    if (code != Parameters::CODE_REGISTER_USER)
        return;

    wSpinnerWidget->start();
    if (DirectoryService::setUserFolder(this)){
        wSpinnerWidget->stop();
        emit userRegistered();
        return;
    }
    //QMessageBox::critical(this, "Zemini Info", "An error occured when setting your folder.");
    wSpinnerWidget->stop();
    this->hide();
}

void RegisterForm::onDbInitialized()
{
    qDebug() << "Stopping the spinner !";
    wSpinnerWidget->stop();

    ui->lb_error_text->setVisible(false);
    ui->lb_username->setStyleSheet("color: black; font-family: Palatino Linotype; font-size: 13px;");
    ui->le_username->setStyleSheet("font-family: Palatino Linotype; font-size: 13px;");

    ui->lb_mail->setStyleSheet("color: black; font-family: Palatino Linotype; font-size: 13px;");
    ui->le_mail->setStyleSheet("font-family: Palatino Linotype; font-size: 13px;");

    ui->lb_password->setStyleSheet("color: black; font-family: Palatino Linotype; font-size: 13px;");
    ui->le_password->setStyleSheet("font-family: Palatino Linotype; font-size: 13px;");
}

void RegisterForm::onRequestFailed(int code)
{
    //qDebug() << "Handling request failure signal " << code << endl;
    if (code == Parameters::CODE_REGISTER_USER){
        ui->lb_error_text->setText("Check your credentials");
        ui->lb_error_text->setVisible(true);

        ui->lb_username->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_username->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

        ui->lb_mail->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_mail->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

        ui->lb_password->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_password->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
    }
    wSpinnerWidget->stop();
}
