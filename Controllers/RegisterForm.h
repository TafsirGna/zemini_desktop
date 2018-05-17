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
    void onUserNotRegistered();

private:
    Ui::RegisterForm *ui;
    QValidator * emailValidator;
    NetworkService * networkService;
    LocalDBService * localDbService;
    ServiceContainer * serviceContainer;
    User * user;
    WaitingSpinnerWidget * wSpinnerWidget;
    QTimer * initDbTimer;

    bool setUserFolder(QDir);

private slots:
    void on_bt_next_clicked();
    void showErrorSignes(QLabel *, QLineEdit *);
    void removeErrorSignes(QLabel *label, QLineEdit *lineEdit);
    void on_le_mail_textChanged(const QString &arg1);
    void on_le_password_textChanged(const QString &arg1);
    void on_lb_sign_in_linkActivated(const QString &link);
    bool isPasswordValid();
    bool isEmailValid();
    void resumeDbInit();
    void onConnectionError(int);
    void onDbInitialized();
    void onRequestFailed(int);
    void onUserSaved(int);


public slots:
    //void handleFocusBack();
    void on_le_username_textChanged(const QString &arg1);

signals:
    void logInLinkActivated();
    void userRegistered();
};

#endif // RegisterForm_H
