#ifndef APP_H__INCLUDED
#define APP_H__INCLUDED

#include "logger.h"
#include <util/singleton.h>
#include <util/element_tree.h>
#include <orm/schema.h>
#include <orm/engine.h>
#include <orm/data_object.h>

#define SETTINGS_FILE "settings.xml"

class AppSettings
{
    Yb::ElementTree::ElementPtr root_;
    std::string file_name_;
    bool modified_;

public:
    AppSettings(const std::string &file_name = SETTINGS_FILE);
    ~AppSettings();

//Common functions
    void fill_tree();
    void save_to_xml();

//Setters
    void set_port(int port);
    void set_db_conn_string(const std::string &db_conn_string);
    void set_logger_mode(bool active);
    void set_log_filename(const std::string &log_filename);

//Getters
    int get_port() const;
    const std::string get_db_conn_string() const;
    bool is_logger_up() const;
    const std::string get_log_filename() const;
    const std::string to_string() const;
};

class App: public Yb::ILogger
{
    Yb::ILogger::Ptr log_;
    std::auto_ptr<Yb::Engine> engine_;
    AppSettings app_settings;

    void init_log();
    void init_engine(Yb::ILogger *root_logger);

public:
    App(const std::string &file_name = SETTINGS_FILE);
    ~App();
    Yb::Engine *get_engine() { return engine_.get(); }
    std::auto_ptr<Yb::Session> new_session();
    Yb::ILogger::Ptr new_logger(const std::string &name);
    void log(int level, const std::string &msg);
    const std::string get_name() const;
    AppSettings &get_settings() { return app_settings; }
};

typedef Yb::SingletonHolder<App> theApp;

#endif //APP_H__INCLUDED

// vim:ts=4:sts=4:sw=4:et:
