#ifndef RegisterForm_H
#define RegisterForm_H

#include <QWidget>
#include <QMessageBox>
#include "Database\Entities\User.h"
#include <QPainter>
#include <QBitmap>
#include <QLineEdit>
#include <QRegExpValidator>
#include "Config/Parameters.h"
#include "Services/NetworkService.h"
#include <QDesktopWidget>

namespace Ui {
    class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0, NetworkService * networkService = 0);
    ~RegisterForm();

private:
    Ui::RegisterForm *ui;
    QValidator * emailValidator;
    NetworkService * networkService;

private slots:
    void on_bt_next_clicked();
    void showErrorSignes(QLabel *, QLineEdit *);
    void removeErrorSignes(QLabel *label, QLineEdit *lineEdit);
    void on_le_mail_textChanged(const QString &arg1);
    void on_le_password_textChanged(const QString &arg1);
    void on_lb_sign_in_linkActivated(const QString &link);
    bool checkPassword();
    bool checkEmail();

public slots:
    //void handleFocusBack();
    void on_le_username_textChanged(const QString &arg1);

signals:
    void userDataEntered(User *);
    void logInLinkActivated();
};

#endif // RegisterForm_H
