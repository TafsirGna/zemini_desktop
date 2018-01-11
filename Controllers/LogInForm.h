#ifndef LogInForm_H
#define LogInForm_H

#include <QWidget>
#include <QMessageBox>
#include "Database\Entities\User.h"
#include "Config\Parameters.h"
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QTimer>
#include <QLineEdit>

namespace Ui {
    class LogInForm;
}

class LogInForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogInForm(QWidget *parent = 0);
    ~LogInForm();

private:
    Ui::LogInForm *ui;
    QNetworkAccessManager *networkAccessManager;
    bool connected;

private slots:
    void on_le_password_textChanged(QString password);
    void handleRequestReply(QNetworkReply *);
    bool checkConnection();
    void showErrorSignes(QLabel *, QLineEdit *);
    void removeErrorSignes(QLabel *label, QLineEdit *);
    void on_le_mail_textChanged(const QString &arg1);
    void on_bt_ok_clicked();

    void on_lb_sign_up_linkActivated(const QString &link);

signals:
    void userDataEntered(User * user);
    void signUpLinkActivated();
};

#endif // LogInForm_H
