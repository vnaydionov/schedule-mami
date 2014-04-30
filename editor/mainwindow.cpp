#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include "ScheduleEditor.h"
#include "SettingsEditor.h"
#include "ReferenceEditor.h"
#include "ScheduleDatesDialog.h"
#include "ActivityDialog.h"
#include "domain/Schedule.h"
#include "domain/ScheduleActivity.h"
#include "domain/Activity.h"
#include "StudGroupDialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(theApp::instance().get_connect_status(), SIGNAL(connected()), this, SLOT(unlock_menu()));
    connect(theApp::instance().get_connect_status(), SIGNAL(disconnected()), this, SLOT(lock_menu()));
    theApp::instance().reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::unlock_menu()
{
    for (int i=0; i < 3; i++)
        ui->FunctionsLW->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

void
MainWindow::lock_menu()
{
    for (int i=0; i < 3; i++)
        ui->FunctionsLW->item(i)->setFlags(Qt::ItemIsEditable);
}

void MainWindow::call_auth_dialog()
{
    bool ok;
    QString text = QInputDialog::getText(0, QString::fromUtf8("Система безопасности"),
                                            QString::fromUtf8("Введите пароль:"),
                                            QLineEdit::Password,
                                            QString::null, &ok);
    if (ok) {
        if (!text.compare("qwerty")) {
            this->setEnabled(true);
        } else {
            QMessageBox::warning(0, QString::fromUtf8("Ошибка"),
                                    QString::fromUtf8("Не верный пароль.."),
                                    QMessageBox::Ok);
            call_auth_dialog();
        }
    } else
        this->destroy();
}

void MainWindow::on_RunDialogBt_clicked()
{
    if (ui->FunctionsLW->currentIndex().row() == 0) {
        ScheduleEditor *sched = new ScheduleEditor(this);
        sched->show();
//    } else if (ui->FunctionsLW->currentIndex().row() == 1) {
//        ui->Description_l->setText(QString::fromUtf8("Выбранный вами модуль находится в разработке."));
    } else if (ui->FunctionsLW->currentIndex().row() == 1) {
        ReferenceEditor *sett = new ReferenceEditor(this);
        sett->show();
    } else if (ui->FunctionsLW->currentIndex().row() == 2) {
        SettingsEditor *sett = new SettingsEditor();
        sett->show();
    } else if (ui->FunctionsLW->currentIndex().row() == 3)
        info();
}

void
MainWindow::info()
{
    const QString info = "This application is developed at MSOU (Moscow State Open University) by:\n"
                         "Viacheslav Fedorov -- funny-story at yandex dot ru\n"
                         "Andrey Skobenkov -- andrey dot skobenkov at gmail dot com\n"
                         "Under the leadership of Viacheslav Naydenov, vaclav at yandex dot ru :)";
    QMessageBox::information(0, "About", info, QMessageBox::Ok);
}


void MainWindow::on_FunctionsLW_itemSelectionChanged()
{
    if (!ui->RunDialogBt->isEnabled())
            ui->RunDialogBt->setEnabled(true);
    if (ui->FunctionsLW->currentIndex().row() == 0) {
        ui->Description_l->setText(QString::fromUtf8("Выбранный вами пункт предлогает вам удобный интерфейс для "
                                                     "редактирования расписаний занятий студентов.\r\n"
                                                     "Внимание! Поскольку модуль работает с базой данных, то "
                                                     "при сохранении внесённых вами изменений они заносятся в базу данных, а "
                                                     "соответствующие записи помечаются как изменённые, что влияет на работу "
                                                     "сервера рассылки расписаний, который в дальнейшем, при получении запроса "
                                                     "с мобильного клиента, отправит ему эти изменения, "
                                                     "так что будьте внимательны к изменённым данным при процедуре сохранения."));
//    } else if (ui->FunctionsLW->currentIndex().row() == 1) {
//        ui->Description_l->setText(QString::fromUtf8("Выбранный вами модуль находится в разработке."));
    } else if (ui->FunctionsLW->currentIndex().row() == 1) {
        ui->Description_l->setText(QString::fromUtf8("В редакторе справочников вы можете дополнять и редактировать имеющиеся справочники, "
                                                     "такие как списки студентов, преподавателей, предметов и др."));
    } else if (ui->FunctionsLW->currentIndex().row() == 2) {
        ui->Description_l->setText(QString::fromUtf8("В настройках программы вы можете управлять аутентификацией и настройками подключение к базе, "
                                                     "а также включить логирование и ведение журнала авторизаций."));
    } else if (ui->FunctionsLW->currentIndex().row() == 3) {
        ui->Description_l->setText(QString::fromUtf8("Эта программа является одновременно редактором и планировщиком расписаний, "
                                                     "что позволяет легко и управлять изменениями в расписании, делая процесс "
                                                     "его редактирования планирования комфортным и лёгким, поскольку все ошибки "
                                                     "и противоречия в расписании программа найдёт сама.\r\n"
                                                     "В состав программы входят редактор и планировщик, взаимодействующие с "
                                                     "сервером рассылки расписаний и мобильным клиентом.\r\n"
                                                     "Теперь процесс планирования и редактирования расписания станет увлекательным и интересным, "
                                                     "а полученное расписание, более надёжным и предостережённым от ошибок.\r\n"
                                                     "Успехов в работе :)"));
    } else if (ui->FunctionsLW->currentIndex().row() == 5) {
        if (QMessageBox::information(0, QString::fromUtf8("Выход из программы"),
                                     QString::fromUtf8("Вы точно хотите выйти из программы?"),
                                     QMessageBox::Cancel, QMessageBox::Ok
                                    ) == QMessageBox::Ok)
            close();
        else {
            ui->RunDialogBt->setEnabled(false);
            ui->FunctionsLW->setCurrentIndex(QModelIndex());
        }
    }
}

void MainWindow::on_FunctionsLW_doubleClicked(const QModelIndex &index)
{
    on_RunDialogBt_clicked();
}
