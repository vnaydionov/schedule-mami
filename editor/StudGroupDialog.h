#ifndef STUDGROUPDIALOG_H
#define STUDGROUPDIALOG_H

#include <QDialog>
#include "domain/StudGroup.h"
#include "domain/Schedule.h"
#include <QStringList>

namespace Ui {
class StudGroupDialog;
}

class StudGroupDialog : public QDialog
{
    Q_OBJECT
    
public:
    QString getGroupsString();
    explicit StudGroupDialog(Domain::ScheduleHolder _schedule, QWidget *parent = 0);
    ~StudGroupDialog();
    
private slots:
    void on_add_But_clicked();

    void on_rm_But_clicked();

    void on_ok_But_clicked();

private:
    void closeEvent(QCloseEvent *);
    void load_item_group();
    void load_all_group();
    Domain::ScheduleHolder schedule;
    Ui::StudGroupDialog *ui;
signals:
    void closeDialog(int);
};

#endif // STUDGROUPDIALOG_H
