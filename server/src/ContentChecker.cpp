#include "ContentChecker.h"

ContentChecker::ContentChecker()
{}

bool
ContentChecker::contentIsValid(StringMap &content) const
{
    try {
        if (!content["&content-type"].compare("text/xml"))
            m_parsedParams = parseParams(content);
        else
            throw std::runtime_error("Unknown content type: " + content["&content-type"]);
    }
    catch (const std::exception &err) {
        m_err = err.what();
        return false;
    }
    return true;
}

StringMap
ContentChecker::parseParams(StringMap &content) const
{
    StringMap params;
    Yb::ElementTree::ElementPtr request_tree = Yb::ElementTree::parse(content["&post-data"]);
    params["version"] = request_tree->get_attr("version");
    params["type"] = request_tree->get_attr("type");
    
    m_requestType = checkRequestType(params["type"]);

    if (m_requestType == UnknownType)
        throw std::runtime_error("Unknown request type: " + params["type"]);
    if (m_requestType == Auth) {
        params["login"] = request_tree->find_first("login")->get_text();
        params["pass"] = request_tree->find_first("pass")->get_text();
    } else {
        params["token"] = request_tree->find_first("token")->get_text();
        if (m_requestType == DownloadSchedule)
            params["start_dt"] = request_tree->find_first("start_dt")->get_text();
        else if (m_requestType == CheckSchedule)
            params["tree-inspections"] = request_tree->serialize();
    }
    
    return params;
}

int
ContentChecker::checkRequestType(const std::string &type) const
{
    const char *type_names[] = {
            "auth", "session_info", "logout", "update", 
            "download", "check", "last_update_info", NULL
            };
    for (int type_code = 0; type_names[type_code]; ++type_code) {
        if (!type.compare(type_names[type_code]))
            return ++type_code;
    }
    return UnknownType;
}

// vim:ts=4:sts=4:sw=4:et:
