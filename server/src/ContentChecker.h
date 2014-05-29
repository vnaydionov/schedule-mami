#ifndef CONTENT_CHECKER__INCLUDED
#define CONTENT_CHECKER__INCLUDED

#include "global.h"

class ContentChecker {
    mutable StringDict m_parsedParams;
    mutable int m_requestType;
    mutable std::string m_err;
    StringDict parseParams(const StringDict &content) const;
    int checkRequestType(const std::string &type) const;
public:
    ContentChecker();
    bool contentIsValid(const StringDict &content) const;
    int requestType() const { return m_requestType; }
    StringDict parsedParams() { return m_parsedParams; }
    std::string err() const { return m_err; }
};

#endif // CONTENT_CHECKER__INCLUDED

// vim:ts=4:sts=4:sw=4:et:

