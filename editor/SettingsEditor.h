#ifndef SETTINGSEDITOR_H
#define SETTINGSEDITOR_H

#include <QDialog>
#include <QtGui>

class SettingsEditor : public QDialog
{
    Q_OBJECT

public:
    SettingsEditor();

public slots:
    void change_page(QListWidgetItem *current, QListWidgetItem *previous);
    void close_dialog();
private:
    void create_icons();

    QListWidget *contents_widget;
    QStackedWidget *pages_widget;
};

class SecuritySettingsPage : public QWidget
{
    Q_OBJECT

private:
    QCheckBox *auth_up_ch;
    QLineEdit *login_tx;
    QLineEdit *pass_tx;
public:
    SecuritySettingsPage(QWidget *parent = 0);

private slots:
    void on_auth_up_click();
    void save_settings();
};

class ConnectionSettingsPage : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *db_url_tx;
    QCheckBox *logger_up_ch;
public:
    ConnectionSettingsPage(QWidget *parent = 0);

private slots:
    void save_settings();
    void check_connect();
};


#endif // SETTINGSEDITOR_H
