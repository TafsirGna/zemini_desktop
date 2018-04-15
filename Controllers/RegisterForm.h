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
#include <Services/ServiceContainer.h>
#include <Controllers/AbstractController.h>
#include <Vendor/QtWaitingSpinner/waitingspinnerwidget.h>

namespace Ui {
    class RegisterForm;
}

class RegisterForm : public QWidget, AbstractController
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0, ServiceContainer * serviceContainer = 0);
    ~RegisterForm();

private:
    Ui::RegisterForm *ui;
    QValidator * emailValidator;
    NetworkService * networkService;
    ServiceContainer * serviceContainer;
    User * user;

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
    void handleUserSavedResponse(bool);

signals:
    void userDataEntered(User *);
    void logInLinkActivated();
    void userToSave(User *);
};

#endif // RegisterForm_H
