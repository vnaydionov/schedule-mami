#include "ScheduleDatesDialog.h"
#include "ui_ScheduleDatesDialog.h"
#include <QInputDialog>
#include <QDebug>
#include "domain/ScheduleDate.h"
#include "domain/Schedule.h"
#include <orm/MetaData.h>
#include <orm/DomainFactory.h>
#include "App.h"

using namespace Yb;
using namespace Domain;

ScheduleDatesDialog::ScheduleDatesDialog(Domain::ScheduleHolder _schedule, QWidget *parent) :
    QDialog(parent), schedule(_schedule),
    ui(new Ui::ScheduleDatesDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    init_data();
    load_type_repeat();
    load_dow();
    load_dates_value();
}

QString ScheduleDatesDialog::get_dow()
{
    return ui->dow_Cb->currentText();
}

void
ScheduleDatesDialog::init_data()
{
    type_repeat["each"] = QString::fromLocal8Bit("Каждый день");
    type_repeat["even"] = QString::fromLocal8Bit("По чётным дням");
    type_repeat["odd"] = QString::fromLocal8Bit("По нечётным дням");
    dow.push_back(QString::fromLocal8Bit("Понедельник"));
    dow.push_back(QString::fromLocal8Bit("Вторник"));
    dow.push_back(QString::fromLocal8Bit("Среда"));
    dow.push_back(QString::fromLocal8Bit("Четверг"));
    dow.push_back(QString::fromLocal8Bit("Пятница"));
    dow.push_back(QString::fromLocal8Bit("Суббота"));
    dow.push_back(QString::fromLocal8Bit("Воскресенье"));
}

void
ScheduleDatesDialog::load_dow()
{
    QVector<QString>::iterator it = dow.begin(); 
    int i = -1;
    for (int j = 0;it != dow.end(); ++it) {
        if (schedule->get_data_object()->assigned_key() && !QString::compare(schedule->dow.value().toLower(), (*it).toLower()))
            i = j;
        j++;
        ui->dow_Cb->addItem(*it);
    }
    if (ui->dow_Cb->count())
        ui->dow_Cb->setCurrentIndex(i);
}

void
ScheduleDatesDialog::load_type_repeat()
{
    QMap<QString, QString>::iterator it = type_repeat.begin();
    int i = -1;
    for (int j = 0;it != type_repeat.end(); ++it) {
        if (schedule->get_data_object()->assigned_key() && !QString::compare(schedule->repeat_type.value().toLower(), it.key().toLower()))
            i = j;
        j++;
        ui->type_repeat_Cb->addItem(it.value(), it.key());
    }
    if (ui->type_repeat_Cb->count())
        ui->type_repeat_Cb->setCurrentIndex(i);
}

void
ScheduleDatesDialog::load_dates_value()
{
    bool has_date = true;
    if (schedule->start_dt.is_null() || schedule->end_dt.is_null())
        has_date = false;
    if (has_date) {
        ui->groupBox->setChecked(true);
        set_enabled_widget();
    }
    if (!schedule->start_dt.is_null())
        ui->begin_Dt->setDate(schedule->start_dt.value().date());
    if (!schedule->end_dt.is_null())
        ui->end_Dt->setDate(schedule->end_dt.value().date());
    ManagedList<ScheduleDate>::iterator it = schedule->dates.begin();
    for (;it != schedule->dates.end(); ++it) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it->dt.value().toString("dd.MM.yy"));
        if (!QString::compare(it->item_type.value(), QString("include")))
            ui->include_Lw->addItem(item);
        else
            if (!QString::compare(it->item_type.value(), QString("exclude")))
                ui->exclude_Lw->addItem(item);
    }
    if (ui->include_Lw->count())
        ui->include_Lw->setCurrentRow(0);
    if (ui->exclude_Lw->count())
        ui->exclude_Lw->setCurrentRow(0);
}

void
ScheduleDatesDialog::set_date_schedule()
{
    if (ui->groupBox->isChecked()) {
        schedule->start_dt = QDateTime(ui->begin_Dt->date(), QTime(0,0));
        schedule->end_dt = QDateTime(ui->end_Dt->date(), QTime(0,0));
        if (ui->type_repeat_Cb->currentIndex() == -1)
            schedule->repeat_type = "";
        else
            schedule->repeat_type = ui->type_repeat_Cb->itemData(ui->type_repeat_Cb->currentIndex(), Qt::UserRole).toString();
        if (ui->dow_Cb->currentIndex() == -1)
            schedule->dow = "";
        else
            schedule->dow = ui->dow_Cb->currentText();
        } else {
        schedule->start_dt = QDateTime(QDate(0,0,0), QTime(0,0));
        schedule->end_dt = QDateTime(QDate(0,0,0), QTime(0,0));
        schedule->repeat_type = schedule->dow = "";
    }
}

void
ScheduleDatesDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    set_date_schedule();
    emit closeDialog(5);
}

ScheduleDatesDialog::~ScheduleDatesDialog()
{
    delete ui;
}

QString
ScheduleDatesDialog::getDatesString()
{
    QString text = "...";
    if (ui->groupBox->isChecked())
        text += ui->begin_Dt->date().toString("dd-MM-yy") +
                                                   " - " + ui->end_Dt->date().toString("dd-MM-yy");
    if (ui->include_Lw->count())
    {
        text += QString::fromUtf8("Включая: ");
        for (int i = 0; i < ui->include_Lw->count(); i++) {
            text += ui->include_Lw->item(i)->text();
            if (ui->include_Lw->count() > i + 1)
                text += ", ";
        }
    }
    if (ui->exclude_Lw->count() && ui->groupBox->isChecked())
    {
        text += QString::fromUtf8("\nКроме: ");
        for (int i = 0; i < ui->exclude_Lw->count(); i++) {
            text += ui->exclude_Lw->item(i)->text();
            if (ui->exclude_Lw->count() > i + 1)
                text += ", ";
        }
    }
    return text;
}

void
ScheduleDatesDialog::set_enabled_widget()
{
    ui->exclude_Lab->setEnabled(ui->groupBox->isChecked());
    ui->exclude_Lw->setEnabled(ui->groupBox->isChecked());
    ui->add_exclude_Bt->setEnabled(ui->groupBox->isChecked());
    ui->rm_exclude_Bt->setEnabled(ui->groupBox->isChecked());
}

void ScheduleDatesDialog::on_groupBox_clicked()
{
    set_enabled_widget();
    if (!ui->groupBox->isChecked()) {
        for (int i = ui->exclude_Lw->count() - 1; i >= 0; --i) {
            ui->exclude_Lw->setCurrentRow(i);
            rm_date_from_widget(ui->exclude_Lw, EXCLUDE_DATE);
        }
    }
    else {
        if (ui->type_repeat_Cb->count())
            ui->type_repeat_Cb->setCurrentIndex(0);
        if (ui->dow_Cb->count())
            ui->dow_Cb->setCurrentIndex(0);
    }
}

void
ScheduleDatesDialog::rm_date_from_widget(QListWidget *widget, int _kind)
{
    if (widget->count()) {
        QString kind;
        if (_kind == INCLUDE_DATE)
            kind = "include";
        else
            if (_kind == EXCLUDE_DATE)
                kind = "exclude";
        ManagedList<ScheduleDate>::iterator it = schedule->dates.begin();
        for (; it != schedule->dates.end(); ++it)
        {
            if (!QString::compare(widget->item(widget->currentRow())->text(), it->dt.value().toString("dd.MM.yy")))
                if (!QString::compare(kind, it->item_type.value()))
                {
                    schedule->dates.erase(it);
                    widget->takeItem(widget->currentRow());
                    break;
                }
        }
    }
}

void
ScheduleDatesDialog::add_date()
{
    QObjectList obj_list = sender()->parent()->children();
    ScheduleDateHolder sched_date(*schedule->get_session());
    sched_date->item_type = sender()->objectName();
    sched_date->dt = QDateTime(((QDateEdit *)obj_list.value(2))->date(), QTime(0,0));
    sched_date->schedule = schedule;
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(sched_date->dt.value().date().toString("dd.MM.yy"));
    if (!QString::compare(sender()->objectName(), QString("include"))) {
        ui->include_Lw->addItem(item);
        ui->include_Lw->setCurrentRow(0);
    }
    else
        if (!QString::compare(sender()->objectName(), QString("exclude"))) {
            ui->exclude_Lw->addItem(item);
            ui->exclude_Lw->setCurrentRow(0);
        }
    ((QDialog *)sender()->parent())->close();
}

void
ScheduleDatesDialog::input_dialog(int _kind)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    dialog->setObjectName(QString("Test"));
    QPushButton *but_ok = new QPushButton(dialog);
    but_ok->setText(QString::fromLocal8Bit("Ок"));
    QIcon add_icon, rm_icon;
    add_icon.addFile(QString::fromUtf8(":/images/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
    rm_icon.addFile(QString::fromUtf8(":/images/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
    but_ok->setIcon(add_icon);
    if (_kind == INCLUDE_DATE)
        but_ok->setObjectName(QString("include"));
    else
        if (_kind == EXCLUDE_DATE)
            but_ok->setObjectName(QString("exclude"));
    connect(but_ok, SIGNAL(clicked()), this, SLOT(add_date()));
    QPushButton *but_cancel = new QPushButton(dialog);
    but_cancel->setText(QString::fromLocal8Bit("Отмена"));
    but_cancel->setIcon(rm_icon);
    connect(but_cancel, SIGNAL(clicked()), dialog, SLOT(close()));
    dialog->setWindowTitle(QString::fromLocal8Bit("Выберите дату"));
    dialog->resize(248, 124);
    dialog->setMinimumSize(QSize(248, 124));
    dialog->setMaximumSize(QSize(248, 124));
    dialog->setModal(true);
    QDateEdit *date_edit = new QDateEdit(dialog);
    date_edit->setObjectName(QString::fromUtf8("schedule_Dt"));
    date_edit->setGeometry(QRect(60, 20, 150, 30));
    date_edit->setDateTime(QDateTime(QDate(2012, 1, 1), QTime(0, 0, 0)));
    date_edit->setCalendarPopup(true);
    but_ok->setGeometry(QRect(10, 70, 100, 30));
    but_cancel->setGeometry(QRect(140, 70, 100, 30));
    QLabel *l = new QLabel(dialog);
    l->setText(QString::fromLocal8Bit("Дата :"));
    l->setGeometry(QRect(10, 20, 50, 30));
    dialog->show();
}

void ScheduleDatesDialog::on_rm_include_Bt_clicked()
{
    rm_date_from_widget(ui->include_Lw, INCLUDE_DATE);
}

void ScheduleDatesDialog::on_add_exclude_Bt_clicked()
{
    input_dialog(EXCLUDE_DATE);
}

void ScheduleDatesDialog::on_add_include_Bt_clicked()
{
    input_dialog(INCLUDE_DATE);
}

void ScheduleDatesDialog::on_rm_exclude_Bt_clicked()
{
    rm_date_from_widget(ui->exclude_Lw, EXCLUDE_DATE);
}

void ScheduleDatesDialog::on_ok_But_clicked()
{
    this->close();
}
