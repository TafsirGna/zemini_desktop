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

namespace Ui {
    class LogInForm;
}

class LogInForm : public QWidget, AbstractController
{
    Q_OBJECT

public:
    explicit LogInForm(QWidget *parent = 0, ServiceContainer *serviceContainer = 0);
    ~LogInForm();

private:
    Ui::LogInForm *ui;

private slots:
    void on_le_password_textChanged(QString password);
    void showErrorSignes(QLabel *, QLineEdit *);
    void removeErrorSignes(QLabel *label, QLineEdit *);
    void on_le_mail_textChanged(const QString &arg1);
    void on_bt_ok_clicked();
    void on_lb_sign_up_linkActivated(const QString &link);
    bool isValidEmail();
    void on_le_mail_editingFinished();
    //void on_le_password_editingFinished();

public slots:
    bool areCredentialsOk(int, User *);

signals:
    void onStart(AbstractController *);
    void userToSave(User *);
    void signUpLinkActivated();
};

#endif // LogInForm_H
