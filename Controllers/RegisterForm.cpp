#include "Controllers\RegisterForm.h"
#include "ui_RegisterForm.h"
#include <QDesktopWidget>

RegisterForm::RegisterForm(QWidget *parent, NetworkService * networkService) :
        QWidget(parent),
        ui(new Ui::RegisterForm)
{
    ui->setupUi(this);

    // Setting the regExpr for email lineEdit
    QRegExp emailRegExpr("^[a-zA-Z0-9._-]+@[a-zA-Z0-9._-]{2,}\\.[a-z]{2,4}$");
    emailValidator = new QRegExpValidator(emailRegExpr, this);

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

    this->networkService = networkService;
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::on_bt_next_clicked()
{
    if (!networkService->isConnected()){

        ui->lb_error_text->setText("Error connexion");
        ui->lb_error_text->setVisible(true);
        showErrorSignes(ui->lb_username, ui->le_username);
        showErrorSignes(ui->lb_mail, ui->le_mail);
        showErrorSignes(ui->lb_password, ui->le_password);
        return;
    }

    //i check if all textedits have been filled
    if (ui->le_username->text().isEmpty())
    {
        ui->lb_error_text->setText("Empty username field");
        showErrorSignes(ui->lb_username, ui->le_username);
        ui->le_username->setFocus();
        return;
    }

    if (!checkEmail())
        return;

    if (!checkPassword())
        return;

    // building a user object with all informations gathered
    User * m_user = new User((*(new QString())), (*(new QString())), ui->le_mail->text(), ui->le_username->text(), ui->le_password->text());

    // sending a post request to put the user in the remote db
    networkService->sendUser(m_user);
}

void RegisterForm::on_le_username_textChanged(const QString &arg1)
{
    // i remove every sign of error previously set
    if (networkService->isConnected())
        removeErrorSignes(ui->lb_username, ui->le_username);
}

void RegisterForm::on_le_mail_textChanged(const QString &arg1)
{
    if (networkService->isConnected())
        removeErrorSignes(ui->lb_mail, ui->le_mail);
}

void RegisterForm::on_le_password_textChanged(const QString &arg1)
{
    if (networkService->isConnected())
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

bool RegisterForm::checkPassword()
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

bool RegisterForm::checkEmail()
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
