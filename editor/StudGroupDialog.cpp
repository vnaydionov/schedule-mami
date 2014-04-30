#include "StudGroupDialog.h"
#include "ui_StudGroupDialog.h"
#include "domain/Schedule.h"
#include "domain/ScheduleGroup.h"
#include "App.h"

using namespace Yb;
using namespace Domain;

StudGroupDialog::StudGroupDialog(ScheduleHolder _schedule, QWidget *parent) :
    QDialog(parent), schedule(_schedule),
    ui(new Ui::StudGroupDialog)
{
    setModal(true);
    ui->setupUi(this);
    load_item_group();
    load_all_group();
}

void
StudGroupDialog::load_item_group()
{
    ManagedList<ScheduleGroup>::iterator it = schedule->groups.begin();
    ui->item_group_Lw->clear();
    for(;it != schedule->groups.end(); ++it)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it->group->name_code);
        item->setData(Qt::UserRole, it->group->id.value());
        ui->item_group_Lw->addItem(item);
    }
    if (ui->item_group_Lw->count())
        ui->item_group_Lw->setCurrentRow(0);
}

void
StudGroupDialog::load_all_group()
{
    DomainResultSet<StudGroup> rs = Yb::query<StudGroup>(*schedule->get_session()).all();
    DomainResultSet<StudGroup>::iterator it = rs.begin();
    ui->all_group_Lw->clear();
    for (int count = 0;it != rs.end(); ++it) {
        bool f = true;
        ManagedList<ScheduleGroup>::iterator sched_group = schedule->groups.begin();
        for(;sched_group != schedule->groups.end(); ++sched_group) {
            if (it->id.value() == sched_group->group->id.value()) {
                f = false;
                count++;
                break;
            }
        }
        if (f) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(it->name_code);
            item->setData(Qt::UserRole, it->id.value());
            ui->all_group_Lw->addItem(item);
        }
    }
    if (ui->all_group_Lw->count())
        ui->all_group_Lw->setCurrentRow(0);
}

void
StudGroupDialog::closeEvent(QCloseEvent *)
{
    emit closeDialog(4);
}

QString
StudGroupDialog::getGroupsString()
{
    QString text = "...";
    if (ui->item_group_Lw->count())
    {
        for(int i=0; i < ui->item_group_Lw->count(); i++)
        {
            text += ui->item_group_Lw->item(i)->text();
            if (ui->item_group_Lw->count() > i+1)
                text += ", ";
        }
    }
    return text;
}

StudGroupDialog::~StudGroupDialog()
{
    delete ui;
}

void StudGroupDialog::on_add_But_clicked()
{
    if (ui->all_group_Lw->count() && ui->all_group_Lw->currentRow() > -1)
    {
        QListWidgetItem *item = ui->all_group_Lw->takeItem(ui->all_group_Lw->currentRow());
        ScheduleGroup sched_group(*schedule->get_session());
        StudGroupHolder group(*schedule->get_session(), item->data(Qt::UserRole).toInt());
        sched_group.group = group;
        sched_group.schedule = schedule;
        ui->item_group_Lw->addItem(item);
        ui->item_group_Lw->setCurrentRow(0);
    }
}

void StudGroupDialog::on_rm_But_clicked()
{
   if (ui->item_group_Lw->count() && ui->item_group_Lw->currentRow() > -1)
    {
        ManagedList<ScheduleGroup>::iterator it = schedule->groups.begin();
        for (;it != schedule->groups.end(); ++it)
        {
            if (it->group->id.value() == ui->item_group_Lw->currentItem()->data(Qt::UserRole).toLongLong())
            {
                QListWidgetItem *item = ui->item_group_Lw->takeItem(ui->item_group_Lw->currentRow());
                ui->all_group_Lw->addItem(item);
                schedule->groups.erase(it);
                break;
            }
        }
        if (ui->item_group_Lw->count())
            ui->item_group_Lw->setCurrentRow(0);
    }
}

void StudGroupDialog::on_ok_But_clicked()
{
    close();
}
