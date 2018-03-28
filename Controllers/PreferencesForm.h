#ifndef PreferencesForm_H
#define PreferencesForm_H

#include <QWidget>
#include "Database/EntityManagers/AbstractManager.h"
#include "Database/EntityManagers/FileTypeManager.h"
#include <QFileDialog>
#include "Database/EntityManagers/UserManager.h"
#include <QFileSystemWatcher>
#include <Services/ServiceContainer.h>

namespace Ui {
    class PreferencesForm;
}

class PreferencesForm : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesForm(QWidget *parent = 0, ServiceContainer * serviceContainer = 0);
    ~PreferencesForm();

private:
    Ui::PreferencesForm *ui;

    FileTypeManager * fileTypeManager;
    UserManager * userManager;

private slots:
    void on_bt_apply_clicked();
    void on_bt_cancel_clicked();
    void on_bt_ok_clicked();
    void on_bt_otherLocation_clicked();
    void on_chkbx_updatecompte_clicked(bool checked);

public slots:
    void handle_current_tab_changed(int);

signals:
    void setupDirectory(QStringList);
};

#endif // PreferencesForm_H
