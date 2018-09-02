#ifndef UPLOADINGFORM_H
#define UPLOADINGFORM_H

#include <QWidget>
#include <QPainter>
#include <QBitmap>
#include <Config/Parameters.h>
#include <Services/ServiceContainer.h>
#include <QMenu>

namespace Ui {
class UploadingForm;
}

class UploadingForm : public QWidget
{
    Q_OBJECT

public:
    explicit UploadingForm(QWidget *parent = 0, ServiceContainer * serviceContainer = 0);
    ~UploadingForm();
    void setTrayIcon(QSystemTrayIcon *);

private slots:
    void on_bt_background_clicked();
    void on_bt_detail_clicked();
    void on_bt_pause_clicked();
    void on_bt_cancel_clicked();
    void onFileBackedUp(File *);
    void onStartSendingFiles(QString, QList<DbEntity *>*);
    void onFileChange(File *);
    void onStartWatchingRootDir();

public slots:

private:
    Ui::UploadingForm *ui;
    double filesDoneSize;
    double totalSize;
    QSystemTrayIcon *trayIcon;
    ServiceContainer * serviceContainer;
    bool paused;
    bool cancelled;
    QMap<QString, QString> uploadingSteps;
    int currentStep;
    bool uploadingFiles;

signals:
    void processPaused();
    void processCancelled();
    void processResumed();
    void processRestarted();
};

#endif // UPLOADINGFORM_H
