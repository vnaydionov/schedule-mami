#include "SettingsEditor.h"
#include <memory>
#include "App.h"

using namespace Yb::ElementTree;

SettingsEditor::SettingsEditor()
{
    contents_widget = new QListWidget;
    contents_widget->setViewMode(QListView::IconMode);
    contents_widget->setIconSize(QSize(50, 50));
    contents_widget->setMovement(QListView::Static);
    contents_widget->setMaximumWidth(150);
    contents_widget->setSpacing(15);

    pages_widget = new QStackedWidget;
    pages_widget->addWidget(new SecuritySettingsPage);
    pages_widget->addWidget(new ConnectionSettingsPage);

    QPushButton *close_bt = new QPushButton(QString::fromUtf8("Закрыть"));
    connect(close_bt, SIGNAL(clicked()), this, SLOT(close_dialog()));

    create_icons();
    contents_widget->setCurrentRow(0);

    QHBoxLayout *h_Layout = new QHBoxLayout;
    h_Layout->addWidget(contents_widget);
    h_Layout->addWidget(pages_widget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(close_bt);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(h_Layout);
    main_layout->addStretch(1);
    main_layout->addSpacing(12);
    main_layout->addLayout(buttonsLayout);
    setLayout(main_layout);

    setWindowTitle(QString::fromUtf8("Настройки"));
    //this->setStyleSheet("background-image: url(images/fon.jpg)");
}
void
SettingsEditor::close_dialog()
{
    close();
    theApp::instance().reset();
}

void
SettingsEditor::create_icons()
{
    QListWidgetItem *security_settings_bt = new QListWidgetItem(contents_widget);
    security_settings_bt->setIcon(QIcon("images/security_settings.png"));
    security_settings_bt->setText(QString::fromUtf8("Аутентификация"));
    security_settings_bt->setTextAlignment(Qt::AlignHCenter);
    security_settings_bt->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *connetc_settings_bt = new QListWidgetItem(contents_widget);
    connetc_settings_bt->setIcon(QIcon("images/connect_settings.png"));
    connetc_settings_bt->setText(QString::fromUtf8("  Настройки БД"));
    connetc_settings_bt->setTextAlignment(Qt::AlignHCenter);
    connetc_settings_bt->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contents_widget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(change_page(QListWidgetItem*,QListWidgetItem*)));
}

void
SettingsEditor::change_page(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pages_widget->setCurrentIndex(contents_widget->row(current));
}

SecuritySettingsPage::SecuritySettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QLabel *login_l = new QLabel(QString::fromUtf8("Логин:"));
    login_tx = new QLineEdit();
    QLabel *pass_l = new QLabel(QString::fromUtf8("Пароль:"));
    pass_tx = new QLineEdit();

    login_tx->setEnabled(false);
    pass_tx->setEnabled(false);

    QGroupBox *packages_group = new QGroupBox(QString::fromUtf8("Настройки аутентификации"));

    auth_up_ch = new QCheckBox(QString::fromUtf8("Включить авторизацию"));
    connect(auth_up_ch, SIGNAL(clicked(bool)), this, SLOT(on_auth_up_click()));

    if (theApp::instance().get_settings().is_auth_up()) {
        auth_up_ch->setChecked(true);
        login_tx->setEnabled(true);
        pass_tx->setEnabled(true);
        login_tx->setText(theApp::instance().get_settings().get_login());
        pass_tx->setText(theApp::instance().get_settings().get_pass());
    }

    QCheckBox *journal_up_ch = new QCheckBox(QString::fromUtf8("Вести журнал авторизации"));
    journal_up_ch->setEnabled(false);

    QPushButton *apply_bt = new QPushButton(QString::fromUtf8("Применить"));
    connect(apply_bt, SIGNAL(clicked()), this, SLOT(save_settings()));

    QGridLayout *packages_layout = new QGridLayout;
    packages_layout->addWidget(login_l, 0, 0);
    packages_layout->addWidget(login_tx, 0, 1);
    packages_layout->addWidget(pass_l, 1, 0);
    packages_layout->addWidget(pass_tx, 1, 1);
    packages_layout->addWidget(auth_up_ch, 2, 0);
    packages_layout->addWidget(journal_up_ch, 3, 0);
    packages_group->setLayout(packages_layout);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(packages_group);
    main_layout->addSpacing(12);
    main_layout->addWidget(apply_bt);
    main_layout->addStretch(1);
    setLayout(main_layout);
}

void
SecuritySettingsPage::on_auth_up_click()
{
    if (auth_up_ch->isChecked()) {
        login_tx->setEnabled(true);
        pass_tx->setEnabled(true);
    } else {
        login_tx->setEnabled(false);
        pass_tx->setEnabled(false);
    }
}

void
SecuritySettingsPage::save_settings()
{
    if (auth_up_ch->isChecked()) {
        theApp::instance().get_settings().set_auth_mode("Up");
        theApp::instance().get_settings().set_login(login_tx->text());
        theApp::instance().get_settings().set_pass(pass_tx->text());
    } else
        theApp::instance().get_settings().set_auth_mode("Down");
        theApp::instance().get_settings().save_to_xml();
        QMessageBox::information(0, QString::fromUtf8("Информация"),
                                QString::fromUtf8("Настройки успешно сохранены!"),
                                QMessageBox::Ok);
}

ConnectionSettingsPage::ConnectionSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QLabel *db_url_l = new QLabel(QString::fromUtf8("База данных:"));
    db_url_tx = new QLineEdit();

    db_url_tx->setText(theApp::instance().get_settings().get_db_url());

    QGroupBox *packages_group = new QGroupBox(QString::fromUtf8("Настройки базы данных"));

    logger_up_ch = new QCheckBox(QString::fromUtf8("Включить логирование"));
    if (theApp::instance().get_settings().is_logger_up())
        logger_up_ch->setChecked(true);

    QPushButton *apply_bt = new QPushButton(QString::fromUtf8("Применить"));
    connect(apply_bt, SIGNAL(clicked()), this, SLOT(save_settings()));

    QPushButton *check_bt = new QPushButton(QString::fromUtf8("Проверить подключение"));
    connect(check_bt, SIGNAL(clicked()), this, SLOT(check_connect()));

    QGridLayout *packages_layout = new QGridLayout;
    packages_layout->addWidget(db_url_l, 0, 0);
    packages_layout->addWidget(db_url_tx, 0, 1);
    packages_layout->addWidget(logger_up_ch, 7, 0);
    packages_group->setLayout(packages_layout);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(packages_group);
    main_layout->addSpacing(12);
    main_layout->addWidget(apply_bt);
    main_layout->addStretch(1);
    main_layout->addWidget(check_bt);
    setLayout(main_layout);
}

void
ConnectionSettingsPage::check_connect()
{

    QString result = QString::fromUtf8("Удалось подключиться к базе данных");
    QString url = db_url_tx->text();
    try {
        Yb::SqlConnection conn(url);
    }
    catch (...)
    {
        result = QString::fromUtf8("Ошибка: не удалось покдлючиться к базе данных");
    }
    QMessageBox::information(0, QString::fromUtf8("Проверка подключения"),
                            result, QMessageBox::Ok);
}

void
ConnectionSettingsPage::save_settings()
{
    if (logger_up_ch->isChecked())
        theApp::instance().get_settings().set_logger_mode("Up");
    else
        theApp::instance().get_settings().set_logger_mode("Down");
    theApp::instance().get_settings().set_db_url(db_url_tx->text());
    theApp::instance().get_settings().save_to_xml();
    QMessageBox::information(0, QString::fromUtf8("Информация"),
                            QString::fromUtf8("Настройки успешно сохранены!"),
                            QMessageBox::Ok);
}
