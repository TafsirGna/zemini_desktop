#include "Controllers/LogInForm.h"
#include "ui_LogInForm.h"

LogInForm::LogInForm(QWidget *parent, ServiceContainer * serviceContainer) :
    QWidget(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);

    wSpinnerWidget = new WaitingSpinnerWidget(this, Qt::ApplicationModal, true);

    this->serviceContainer = serviceContainer;

    ui->le_mail->setFocus();
    ui->le_password->setEchoMode(QLineEdit::Password);

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

    //hide the error label
    ui->lb_error_text->setVisible(false);
    ui->lb_error_text->setStyleSheet("color: red;");

    // This commands put the form on the middle of the screen
    QDesktopWidget bureau;
    QRect surface_bureau = bureau.screenGeometry();
    int x = surface_bureau.width()/2 - width()/2;
    int y = surface_bureau.height()/2 - height()/2;
    move(x,y);

    // Prevent the resizing of the window
    setMinimumSize(size());
    setMaximumSize(size());

    //connecting signals to slots
    LocalDBService *localDbService = (LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase);
    NetworkService *networkService = (NetworkService *)this->serviceContainer->getService(ZeminiService::Network);
    connect(localDbService, SIGNAL(userSaved(int)), this, SLOT(onUserSaved(int)));
    connect(networkService, SIGNAL(connectionError(int)), this, SLOT(onConnectionError(int)));
    connect(networkService, SIGNAL(requestFailed(int)), this, SLOT(onRequestFailed(int)));
}

LogInForm::~LogInForm()
{
    delete ui;
}

void LogInForm::waiting()
{
    wSpinnerWidget->start();
}

void LogInForm::on_le_password_textChanged(QString )
{
    if (ui->lb_error_text->isVisible() || ui->lb_error_text->text() == "Error connection"){
        return;
    }
    removeErrorSignes(ui->lb_password, ui->le_password);
}

void LogInForm::on_le_mail_textChanged(const QString &arg1)
{
    if (ui->lb_error_text->isVisible() || ui->lb_error_text->text() == "Error connection"){
        return;
    }

    removeErrorSignes(ui->lb_mail, ui->le_mail);
}

void LogInForm::showErrorSignes(QLabel * label, QLineEdit * lineEdit)
{
    ui->lb_error_text->setVisible(true);
    label->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
    lineEdit->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
}

void LogInForm::removeErrorSignes(QLabel * label, QLineEdit * lineEdit)
{
    if (ui->lb_error_text->isVisible()){
        ui->lb_error_text->setVisible(false);
        label->setStyleSheet("color: black; font-family: Palatino Linotype; font-size: 13px;");
        lineEdit->setStyleSheet("font-family: Palatino Linotype; font-size: 13px;");
    }
}

void LogInForm::on_bt_ok_clicked()
{
    wSpinnerWidget->start();
    if (!isValidEmail()){
        wSpinnerWidget->stop();
        ui->lb_error_text->setText("Wrong email format");
        showErrorSignes(ui->lb_mail, ui->le_mail);
        ui->le_mail->setFocus();
        return;
    }

    // sending a request to the server to check the credentials
    qDebug() << "checking credentials" << endl;
    ((NetworkService*) this->serviceContainer->getService(ZeminiService::Network))->checkCredentials(ui->le_mail->text(), ui->le_password->text());
}

void LogInForm::on_lb_sign_up_linkActivated(const QString &link)
{
    emit signUpLinkActivated();
}

bool LogInForm::isValidEmail()
{
    return Parameters::emailRegExpr->exactMatch(ui->le_mail->text());
}

bool LogInForm::areCredentialsOk(int resultCode, User * user)
{
    if (user == NULL){   // the user wasn't in the remote database

        if (resultCode == 1)
            ui->lb_error_text->setText("Bad credentials");
        else
            ui->lb_error_text->setText("Error connection");

        ui->lb_error_text->setVisible(true);
        showErrorSignes(ui->lb_mail, ui->le_mail);
        showErrorSignes(ui->lb_password, ui->le_password);
        return false;
    }

    // saving the user in the local database
    //emit userToSave(user);

    this->hide();
    //TYPE* dynamic_cast<TYPE*>(object);
    return true;
}

void LogInForm::onConnectionError(int code)
{
    if (code == NetworkService::CODE_USER_LOGIN || code == NetworkService::CODE_DB_INIT){
        wSpinnerWidget->stop();
        // notifying the error to the user
        ui->lb_error_text->setText("Error connection");
        ui->lb_error_text->setVisible(true);

        ui->lb_mail->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_mail->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

        ui->lb_password->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_password->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
    }
}

void LogInForm::onRequestFailed(int code)
{
    if (code == NetworkService::CODE_USER_LOGIN){
        // notitfy ti the user that the request failed
        wSpinnerWidget->stop();
        QMessageBox::warning(this, "Zemini Info", "Oups! It seems that an error occured when logging you in. Please, try later!");
    }
}

void LogInForm::onUserSaved(int code)
{
    if (code != NetworkService::CODE_USER_LOGIN)
        return;

    wSpinnerWidget->start();
    if (DirectoryService::setUserFolder(this)){
        wSpinnerWidget->stop();
        this->hide();
        emit userLoggedIn();
        return;
    }

    QMessageBox::information(0, "Zemini Info", "You decided not to choose the directory for now. You can try later!", QMessageBox::Ok);
    wSpinnerWidget->stop();
    this->hide();
    emit closeAppSignal();
}

void LogInForm::on_le_mail_editingFinished()
{
    if (!((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->isDbInitialized()){
        onConnectionError(NetworkService::CODE_USER_LOGIN);
        return;
    }
    ui->le_password->setFocus();
}


void LogInForm::on_le_password_editingFinished()
{
    if (!((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->isDbInitialized()){
        onConnectionError(NetworkService::CODE_USER_LOGIN);
        return;
    }
}

