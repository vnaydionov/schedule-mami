#include "App.h"
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace Yb;
using namespace ElementTree;

App::App(const std::string &file_name)
    : app_settings(file_name)
{
    init_log();
    init_engine(log_.get());
}

App::~App()
{
    log_->info("log ended");
    engine_.reset(NULL);
    log_.reset(NULL);
}

void 
App::init_log()
{
    if (!log_.get()) {
        log_.reset(new Log(app_settings.get_log_filename()));
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
        //pool->add_source(Yb::Engine::sql_source_from_env("schedule_db"));
        //pool->add_source(Yb::SqlSource("mysql://schedule_usr:schedule_pwd@schedule_db")); 
        Yb::SqlSource src(app_settings.get_db_conn_string());
        src["&id"] = "schedule_db";
        pool->add_source(src);
        engine_.reset(new Yb::Engine(Yb::Engine::MANUAL, pool, "schedule_db"));
        engine_->set_echo(true);
        engine_->set_logger(yb_logger);
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

//------AppSettings---------

AppSettings::AppSettings(const std::string &file_name)
    : file_name_(file_name)
    , modified_(false)
{
    fill_tree();
}

AppSettings::~AppSettings()
{
    if (modified_)
        save_to_xml();
}

//Common functions

void
AppSettings::fill_tree()
{
    std::ifstream file(file_name_.c_str());
    if (file.good()) {
        root_ = ElementTree::parse(file);
        modified_ = false;
    } else {
        root_ = new_element("Settings");
        //Read from settings.xml and substitution in elements.
        //If file isn't existing that writing default values.
        root_->sub_element("port", "19090");
        root_->sub_element("db_conn_string",
                "mysql+odbc://schedule_usr:schedule_pwd@schedule_db");
        root_->sub_element("logger_mode", "Up");
        root_->sub_element("log_filename", "log.txt");
        modified_ = true;

        save_to_xml();
    }
}

void
AppSettings::save_to_xml()
{
    std::ofstream file(file_name_.c_str());
    if (file.good()) {
        file << to_string() << std::endl;
        file.close();
        modified_ = false;
    } else
        std::cerr << "couldn't open '" << file_name_ << "' for writing" << std::endl;
}

//Setters

void
AppSettings::set_port(int port)
{
    root_->find_first("port")->set_text(boost::lexical_cast<std::string>(port));
    modified_ = true;
}

void
AppSettings::set_db_conn_string(const std::string &db_conn_string)
{
    root_->find_first("db_conn_string")->set_text(db_conn_string);
    modified_ = true;
}

void
AppSettings::set_logger_mode(bool active)
{
    root_->find_first("logger_mode")->set_text(active? "Up": "Down");
    modified_ = true;
}

void
AppSettings::set_log_filename(const std::string &log_filename)
{
    root_->find_first("log_filename")->set_text(log_filename);
    modified_ = true;
}

//Getters

int
AppSettings::get_port() const
{
    return boost::lexical_cast<int>(root_->find_first("port")->get_text());
}

const std::string
AppSettings::get_db_conn_string() const
{
    return root_->find_first("db_conn_string")->get_text();
}

bool
AppSettings::is_logger_up() const
{
    return !root_->find_first("logger_mode")->get_text().compare("Up");
}

const std::string
AppSettings::get_log_filename() const
{
    return root_->find_first("log_filename")->get_text();
}

const std::string
AppSettings::to_string() const
{
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" +
        root_->serialize();
}

// vim:ts=4:sts=4:sw=4:et:
