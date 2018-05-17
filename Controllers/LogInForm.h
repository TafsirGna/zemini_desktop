#ifndef LogInForm_H
#define LogInForm_H

#include <QWidget>
#include <QMessageBox>
#include "Database/Entities/User.h"
#include "Config/Parameters.h"
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QTimer>
#include <QLineEdit>
#include <QDesktopWidget>
#include <Services/ServiceContainer.h>
#include <Controllers/AbstractController.h>
#include <Vendor/QtWaitingSpinner/waitingspinnerwidget.h>

namespace Ui {
    class LogInForm;
}

class LogInForm : public QWidget, AbstractController
{
    Q_OBJECT

public:
    explicit LogInForm(QWidget *parent = 0, ServiceContainer *serviceContainer = 0);
    ~LogInForm();
    void waiting();

private:
    Ui::LogInForm *ui;
    WaitingSpinnerWidget * wSpinnerWidget;

private slots:
    void on_le_password_textChanged(QString password);
    void showErrorSignes(QLabel *, QLineEdit *);
    void removeErrorSignes(QLabel *label, QLineEdit *);
    void on_le_mail_textChanged(const QString &arg1);
    void on_bt_ok_clicked();
    void on_lb_sign_up_linkActivated(const QString &link);
    bool isValidEmail();
    void on_le_mail_editingFinished();
    void on_le_password_editingFinished();

public slots:
    bool areCredentialsOk(int, User *);

private slots:
    void onConnectionError( int );
    void onRequestFailed(int);
    void onUserSaved(int);

signals:
    void signUpLinkActivated();
    void userLoggedIn();
    void closeAppSignal();
};

#endif // LogInForm_H
