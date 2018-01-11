#include "Controllers\LogInForm.h"
#include "ui_LogInForm.h"

LogInForm::LogInForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);

    ui->le_password->setEchoMode(QLineEdit::Password);

    ui->le_mail->setFocus();

    // display the user default image on the top of the form
    QPixmap pixmap(Parameters::devAppFolder() + "\\Resources\\Images\\user_icon.png");

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

    // send a get request to check if there's an internet connexion
    networkAccessManager = new QNetworkAccessManager(this);
    QObject::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
    this->connected = false;
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::url())));
    // set a timer that's used to check the connexion
    QTimer::singleShot(1000, this, SLOT(checkConnection()));
}

LogInForm::~LogInForm()
{
    delete ui;
}

void LogInForm::on_le_password_textChanged(QString )
{
    if (connected)
        removeErrorSignes(ui->lb_password, ui->le_password);
}

void LogInForm::handleRequestReply(QNetworkReply *)
{
    this->connected = true;
    qDebug() << "Connection established1!" << endl;
}

bool LogInForm::checkConnection()
{
    if (!connected)
    {
        ui->lb_error_text->setText("Error connexion");
        ui->lb_error_text->setVisible(true);

        ui->lb_mail->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_mail->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");

        ui->lb_password->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
        ui->le_password->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
    }
    return connected;
}

void LogInForm::on_le_mail_textChanged(const QString &arg1)
{
    if (connected)
        removeErrorSignes(ui->lb_mail, ui->le_mail);
}

void LogInForm::showErrorSignes(QLabel * label, QLineEdit * lineEdit)
{
    ui->lb_error_text->setVisible(true);
    label->setStyleSheet("color: red; font-family: Palatino Linotype; font-size: 13px; ");
    lineEdit->setStyleSheet("border-color: red; border-width: 1px; border-style: solid; font-family: Palatino Linotype; font-size: 13px;");
    lineEdit->setFocus();
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
    if (!checkConnection())
        return;

    // i send a request to the server to check the credentials
}

void LogInForm::on_lb_sign_up_linkActivated(const QString &link)
{
    emit signUpLinkActivated();
}
