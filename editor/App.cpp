#include "App.h"
#include <util/string_utils.h>
#include <QFile>
#include <QTextStream>
#include <QtXml>
#include <QMessageBox>
#include <QPushButton>

using namespace Yb;
using namespace ElementTree;

App::App()
{
    connect_status = new ConnectDbStatus();
    init_log();
    init_engine(log_.get());
    QFile file("table_widget_config.xml");
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        tbl_config = Yb::ElementTree::parse(file.readAll().data());
        file.close();
    } else
        QMessageBox::warning(0, QObject::trUtf8("Предупреждение"), QObject::trUtf8("Не найден файл с настройками таблицы.."), QMessageBox::Ok);
}

void
App::save_table_widget_config()
{
    if (tbl_config.get()) {
        std::string s = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + tbl_config->serialize();
        QFile file("table_widget_config.xml");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(s.c_str(), s.size());
            file.close();
        }
    }
}

App::~App()
{
    delete connect_status;
    log_->info("log ended");
    engine_.reset(NULL);
    log_.reset(NULL);
    save_table_widget_config();
}

void
App::reset()
{
    engine_.reset(NULL);
    init_log();
    init_engine(log_.get());
}

void 
App::init_log()
{
    if ( !log_.get() )
    {
        log_.reset(new Log(app_settings.get_log_filename().toStdString()));
        log_->info("log started");
    }
}

void 
App::init_engine(Yb::ILogger *root_logger)
{
    if (!engine_.get()) {
        Yb::ILogger::Ptr yb_logger(root_logger->new_logger("yb").release());
        Yb::init_schema();
        std::auto_ptr<Yb::SqlPool> pool(
                new Yb::SqlPool(YB_POOL_MAX_SIZE, YB_POOL_IDLE_TIME,
                    YB_POOL_MONITOR_SLEEP, yb_logger.get()));
        QString url = app_settings.get_db_url();
        if (url.isEmpty())
            emit connect_status->disconnected();
        else {
            Yb::SqlSource src(url);
            pool->add_source(src);
            try
            {
               pool->get(src.id());
               engine_.reset(new Yb::Engine(Yb::Engine::READ_WRITE, pool, src.id()));
               engine_->set_echo(true);
               engine_->set_logger(yb_logger);
               emit connect_status->connected();
            }
            catch (std::exception &x)
            {
               emit connect_status->disconnected();
              //  const QString info = "Can't connect to database, please check connection settings..";
              //  QMessageBox::information(0, "Connection error", info, QMessageBox::Ok);
            }
        }
    }
}

std::auto_ptr<Yb::Session> 
App::new_session()
{
    return std::auto_ptr<Yb::Session>(
            new Yb::Session(Yb::theSchema(), get_engine()));
}

Yb::ILogger::Ptr 
App::new_logger(const std::string &name)
{
    return log_->new_logger(name);
}

void 
App::log(int level, const std::string &msg)
{
    return log_->log(level, msg);
}

const std::string 
App::get_name() const
{
    return log_->get_name();
}

AppSettings::AppSettings(const QString &file_name_)
    : file_name(file_name_)
    , modified_(false)
{
    fill_tree();
}

AppSettings::~AppSettings()
{
    if (modified_ && root.get())
        save_to_xml();
}

Yb::ElementTree::Element *
AppSettings::checked_root()
{
    if (!root.get())
        throw std::runtime_error("empty settings tree");
    return root.get();
}

QString
AppSettings::toString()
{
    return QString::fromStdString(checked_root()->serialize());
}

void
AppSettings::set(const QString &section, const QString &key, const QString &value)
{
    try {
        QString x = checked_root()->find_first(section)->find_first(key)->get_text();
        if (x != value) {
            checked_root()->find_first(section)->find_first(key)->set_text(value);
            modified_ = true;
        }
    }
    catch (...)
    {
        const QString info = "File settings.xml is bad..fox this problem you may remove him and this application..";
        QMessageBox::information(0, "Settings error", info, QMessageBox::Ok);
    }
}

QString
AppSettings::get(const QString &section, const QString &key)
{
    QString temp;
    try {
        temp = checked_root()->find_first(section)->find_first(key)->get_text();
    }
    catch (...)
    {
        temp = "";
    }
    return temp;
}

void
AppSettings::fill_tree()
{
    QFile file(file_name);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        //Read from settings.xml and substitution in elements.
        root = parse(QString::fromUtf8(file.readAll()).toStdString());
        file.close();
        modified_ = false;
    } else {
        //If file isn't existing that use default values without auth.
        root = new_element("Settings");
        ElementPtr security = root->sub_element("Security");
        security->sub_element("authentication", "Down");
        security->sub_element("login", "");
        security->sub_element("pass", "");
        ElementPtr database = root->sub_element("Database");
        database->sub_element("db_url", "mysql+odbc://schedule_usr:schedule_pwd@schedule_db");
        database->sub_element("logger_mode", "Down");
        modified_ = true;
    }
}

void
AppSettings::save_to_xml()
{
    QString settings_str = toString();
    QFile file(file_name);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << settings_str;
        file.close();
        modified_ = false;
    }
}
