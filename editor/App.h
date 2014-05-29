#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <orm/xmlizer.h>
#include "logger.h"
#include <util/element_tree.h>
#include <util/singleton.h>
#include <orm/schema.h>
#include <orm/engine.h>
#include <orm/data_object.h>
#include <QString>

#define SETTINGS_FILE "settings.xml"
#define TABLE_SETTINGS_FILE "table_widget_settings.xml"

class AppSettings
{
    QString file_name;
    Yb::ElementTree::ElementPtr root;
    bool modified_;

    Yb::ElementTree::Element *checked_root();
    void fill_tree();

public:
    AppSettings(const QString &file_name_ = SETTINGS_FILE);
    ~AppSettings();

    void set(const QString &section, const QString &key, const QString &value);
    QString get(const QString &section, const QString &key);

    void set_auth_mode(const QString &mode) { set("Security", "authentication", mode); }
    void set_login(const QString &login) { set("Security", "login", login); }
    void set_pass(const QString &pass) { set("Security", "pass", pass); }
    void set_db_url(const QString &db_url) { set("Database", "db_url", db_url); }
    void set_logger_mode(const QString &mode) { set("Database", "logger_mode", mode); }

    QString get_login() { return get("Security", "login"); }
    QString get_pass() { return get("Security", "pass"); }
    QString get_db_url() { return get("Database", "db_url"); }
    QString get_log_filename() { return "log.txt"; }

    bool is_auth_up() { return get("Security", "authentication") == "Up"; }
    bool is_logger_up() { return get("Database", "logger_mode") == "Up"; }

    QString toString();
    void save_to_xml();
};

class ConnectDbStatus;

class App: public Yb::ILogger
{
    Yb::ILogger::Ptr log_;
    std::auto_ptr<Yb::Engine> engine_;
    AppSettings app_settings;
    Yb::ElementTree::ElementPtr tbl_config;
    void init_log();
    void init_engine(Yb::ILogger *root_logger);
    ConnectDbStatus *connect_status;
public:
    App();
    ~App();
    void reset();
    ConnectDbStatus *get_connect_status() { return connect_status; }
    Yb::Engine *get_engine() { return engine_.get(); }
    std::auto_ptr<Yb::Session> new_session();
    Yb::ILogger::Ptr new_logger(const std::string &name);
    void log(int level, const std::string &msg);
    void save_table_widget_config();
    const std::string get_name() const;
    AppSettings &get_settings() { return app_settings; }
    Yb::ElementTree::ElementPtr &get_table_config() { return tbl_config; }        
};

class ConnectDbStatus: public QObject
{
    Q_OBJECT
    friend class App;
public:
    ConnectDbStatus() : QObject() { }
    ~ConnectDbStatus() { }
signals:
    void connected();
    void disconnected();
};

typedef Yb::SingletonHolder<App> theApp;

#endif // APPSETTINGS_H
