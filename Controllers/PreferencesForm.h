#ifndef ZEMINIPREFERENCESFORM_H
#define ZEMINIPREFERENCESFORM_H

#include <QWidget>
#include "Database\EntityManagers\AbstractManager.h"
#include "Database\EntityManagers\TypeManager.h"
#include <QFileDialog>
#include "Database\EntityManagers\UserManager.h"
#include "Database\EntityManagers\DirectoryManager.h"
#include <QFileSystemWatcher>

namespace Ui {
    class ZeminiPreferencesForm;
}

class ZeminiPreferencesForm : public QWidget
{
    Q_OBJECT

public:
    explicit ZeminiPreferencesForm(QList<AbstractManager> * listEntitiesManager,QWidget *parent = 0, QSqlDatabase * db = 0);
    ~ZeminiPreferencesForm();

private:
    Ui::ZeminiPreferencesForm *ui;

    TypeManager * typeManager;
    UserManager * userManager;
    DirectoryManager * dirManager;
    Directory * otherDir;

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

#endif // ZEMINIPREFERENCESFORM_H
