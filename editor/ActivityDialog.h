#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include "domain/Schedule.h"

namespace Ui {
class ActivityDialog;
}

class ActivityDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ActivityDialog(Domain::ScheduleHolder _schedule, QWidget *parent=0);
    ~ActivityDialog();
    QString getActivitiesString();
private slots:
    void on_add_But_clicked();
    void on_rm_But_clicked();
    void on_ok_But_clicked();
private:
    void show_append_dialog();
    void closeEvent(QCloseEvent *);
    Domain::ScheduleHolder schedule;
    void drag_drop_item(QListWidget *from, QListWidget *to);
    void load_all_activity();
    void load_item_activity();
    void load_subjects();
    Ui::ActivityDialog *ui;
signals:
    void closeDialog(int);
};

#endif // ACTIVITYDIALOG_H
