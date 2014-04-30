#ifndef CONTENT_CHECKER__INCLUDED
#define CONTENT_CHECKER__INCLUDED

#include "global.h"

class ContentChecker {
    mutable StringMap m_parsedParams;
    mutable int m_requestType;
    mutable std::string m_err;
    StringMap parseParams(StringMap &content) const;
    int checkRequestType(const std::string &type) const;
public:
    ContentChecker();
    bool contentIsValid(StringMap &content) const;
    int requestType() const { return m_requestType; }
    StringMap parsedParams() { return m_parsedParams; }
    std::string err() const { return m_err; }
};

#endif // CONTENT_CHECKER__INCLUDED

// vim:ts=4:sts=4:sw=4:et:

