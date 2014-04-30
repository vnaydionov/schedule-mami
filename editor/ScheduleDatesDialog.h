#ifndef SCHEDULEDATESDIALOG_H
#define SCHEDULEDATESDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QCloseEvent>
#include "domain/Schedule.h"

enum {INCLUDE_DATE = 1, EXCLUDE_DATE = 2};

namespace Ui {
    class ScheduleDatesDialog;
}

class ScheduleDatesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScheduleDatesDialog(Domain::ScheduleHolder _schedule, QWidget *parent = 0);
    ~ScheduleDatesDialog();
    QString getDatesString();
    QString get_dow();
private slots:
    void on_groupBox_clicked();
    void rm_date_from_widget(QListWidget *widget, int _kind);
    void on_rm_include_Bt_clicked();

    void on_add_exclude_Bt_clicked();
    void closeEvent(QCloseEvent *event);
    void input_dialog(int _kind);
    void on_add_include_Bt_clicked();
    void add_date();
    void set_enabled_widget();
    void on_rm_exclude_Bt_clicked();
    void set_date_schedule();
    void load_dates_value();
    void load_type_repeat();
    void load_dow();
    void init_data();
    void on_ok_But_clicked();
private:
    Domain::ScheduleHolder schedule;
    QMap<QString, QString> type_repeat;
    QVector<QString> dow;
    Ui::ScheduleDatesDialog *ui;
signals:
    void closeDialog(int);
};

#endif // SCHEDULEDATESDIALOG_H
