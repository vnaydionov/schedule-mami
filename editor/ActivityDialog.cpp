#include "ActivityDialog.h"
#include "ui_ActivityDialog.h"
#include "domain/Activity.h"
#include "domain/ScheduleActivity.h"
#include "App.h"
#include <QDebug>

using namespace Yb;
using namespace Domain;

ActivityDialog::ActivityDialog(ScheduleHolder _schedule, QWidget *parent) :
    QDialog(parent), schedule(_schedule),
    ui(new Ui::ActivityDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    load_item_activity();
    load_all_activity();
    load_subjects();
}

void
ActivityDialog::load_item_activity()
{
    Yb::ManagedList<ScheduleActivity>::iterator it = schedule->activities.begin();
    ui->item_activity_Lw->clear();
    for(;it != schedule->activities.end(); ++it)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it->activity->name);
        item->setData(Qt::UserRole, it->activity->id.value());
        ui->item_activity_Lw->addItem(item);
    }
    if (ui->item_activity_Lw->count())
        ui->item_activity_Lw->setCurrentRow(0);
}

void
ActivityDialog::load_all_activity()
{
    DomainResultSet<Activity> rs = Yb::query<Activity>(*schedule->get_session()).all();
    DomainResultSet<Activity>::iterator it = rs.begin();
    ui->all_activity_Lw->clear();
    for (unsigned int count = 0;it != rs.end(); ++it) {
        bool f = true;
        ManagedList<ScheduleActivity>::iterator sched_activity = schedule->activities.begin();
        for(;sched_activity != schedule->activities.end() && count != schedule->activities.size(); ++sched_activity) {
            if (it->id.value() == sched_activity->activity->id.value()) {
                f = false;
                count++;
                break;
            }
        }
        if (f) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(it->name);
            item->setData(Qt::UserRole, it->id.value());
            ui->all_activity_Lw->addItem(item);
        }
    }
    if (ui->all_activity_Lw->count())
        ui->all_activity_Lw->setCurrentRow(0);
}


void
ActivityDialog::load_subjects()
{
    DomainResultSet<Subject> rs = Yb::query<Subject>(*schedule->get_session()).all();
    DomainResultSet<Subject>::iterator it = rs.begin();
    ui->subject_Cb->clear();
    int current_subj_index = 0;
    for (int i = 0;it != rs.end(); ++it, i++) {
        ui->subject_Cb->addItem(it->name);
        ui->subject_Cb->setItemData(i, it->id.value(), Qt::UserRole);
        if (schedule->subject->get_data_object()->assigned_key() && it->id.value() == schedule->subject->id.value())
            current_subj_index = i;
    }
    if (ui->subject_Cb->count())
        ui->subject_Cb->setCurrentIndex(current_subj_index);
}

void
ActivityDialog::closeEvent(QCloseEvent *)
{
    emit closeDialog(3);
}

ActivityDialog::~ActivityDialog()
{
    delete ui;
}

QString
ActivityDialog::getActivitiesString()
{
    QString text = "...";
    text += ui->subject_Cb->currentText();
    if (ui->item_activity_Lw->count()) {
        text += "\n";
        for(int i=0; i < ui->item_activity_Lw->count(); i++) {
            if (ui->item_activity_Lw->count() > 0)
                text += ", ";
            text += ui->item_activity_Lw->item(i)->text();
        }
    }
    return text;
}

void ActivityDialog::drag_drop_item(QListWidget *from, QListWidget *to)
{
    if (from->count() && from->currentRow() > -1)
    {
        QListWidgetItem *item = new QListWidgetItem(*from->currentItem());
        to->addItem(item);
        from->takeItem(from->currentRow());
    }
}

void ActivityDialog::on_add_But_clicked()
{
   if (ui->all_activity_Lw->count() && ui->all_activity_Lw->currentRow() > -1)
    {
        QListWidgetItem *item = ui->all_activity_Lw->takeItem(ui->all_activity_Lw->currentRow());
        ScheduleActivity sched_activ(*schedule->get_session());
        ActivityHolder activity(*schedule->get_session(), item->data(Qt::UserRole).toLongLong());
        sched_activ.activity = activity;
        sched_activ.schedule = schedule;
        ui->item_activity_Lw->addItem(item);
        ui->item_activity_Lw->setCurrentRow(0);
    }
}

void ActivityDialog::on_rm_But_clicked()
{
    if (ui->item_activity_Lw->count() && ui->item_activity_Lw->currentRow() > -1)
    {
        ManagedList<ScheduleActivity>::iterator it = schedule->activities.begin();
        for (;it != schedule->activities.end(); ++it)
        {
            if (it->activity->id.value() == ui->item_activity_Lw->currentItem()->data(Qt::UserRole).toLongLong())
            {
                QListWidgetItem *item = ui->item_activity_Lw->takeItem(ui->item_activity_Lw->currentRow());
                ui->all_activity_Lw->addItem(item);
                schedule->activities.erase(it);
                break;
            }
        }
        if (ui->item_activity_Lw->count())
            ui->item_activity_Lw->setCurrentRow(0);
    }
}

void ActivityDialog::on_ok_But_clicked()
{
    qDebug() << ui->subject_Cb->itemData(ui->subject_Cb->currentIndex(), Qt::UserRole).toLongLong();
    SubjectHolder subj(*schedule->get_session(), ui->subject_Cb->itemData(ui->subject_Cb->currentIndex(), Qt::UserRole).toLongLong());
    schedule->subject = subj;
    close();
}
