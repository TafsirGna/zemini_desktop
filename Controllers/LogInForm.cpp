#include "Controllers/LogInForm.h"
#include "ui_LogInForm.h"

LogInForm::LogInForm(QWidget *parent, ServiceContainer * serviceContainer) :
    QWidget(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);

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

    // connecting signals to slots
    connect(((NetworkService*) this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(requestReplyReceived(QMap<QString,QString>,QList<DbEntity>*)), this, SLOT(onRequestReplyReceived(QMap<QString,QString>,QList<DbEntity>*)));
}

LogInForm::~LogInForm()
{
    delete ui;
}

void LogInForm::on_le_password_textChanged(QString )
{
    if (((NetworkService*) this->serviceContainer->getService(ZeminiService::Network))->isConnected())
        removeErrorSignes(ui->lb_password, ui->le_password);
}

void LogInForm::on_le_mail_textChanged(const QString &arg1)
{
    if (((NetworkService*) this->serviceContainer->getService(ZeminiService::Network))->isConnected())
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
    if (!((NetworkService*) this->serviceContainer->getService(ZeminiService::Network))->isConnected()){

        ui->lb_error_text->setText("Error connexion");
        ui->lb_error_text->setVisible(true);
        showErrorSignes(ui->lb_mail, ui->le_mail);
        showErrorSignes(ui->lb_password, ui->le_password);
        return;
    }

    if (!isValidEmail()){
        ui->lb_error_text->setText("Wrong email format");
        showErrorSignes(ui->lb_mail, ui->le_mail);
        ui->le_mail->setFocus();
        return;
    }

    // sending a request to the server to check the credentials
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
    emit userToSave(user);

    this->hide();
    //TYPE* dynamic_cast<TYPE*>(object);
    return true;
}

void LogInForm::on_le_mail_editingFinished()
{
    ui->le_password->setFocus();
}

/*
void LogInForm::on_le_password_editingFinished()
{
    qDebug() << " significatif : " <<endl;
    on_bt_ok_clicked();
}
*/
