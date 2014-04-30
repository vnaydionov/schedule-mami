#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include "App.h"
#include "domain/Schedule.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void unlock_menu();
    void lock_menu();
    void on_RunDialogBt_clicked();
    void info();
    void on_FunctionsLW_itemSelectionChanged();

    void on_FunctionsLW_doubleClicked(const QModelIndex &index);

public slots:
    void call_auth_dialog();

private:
    std::auto_ptr<Yb::Session> session;
    Domain::ScheduleHolder *sched;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
