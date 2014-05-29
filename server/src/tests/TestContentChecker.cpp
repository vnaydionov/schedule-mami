#include <cppunit/extensions/HelperMacros.h>
#include "../ContentChecker.h"
#include "../global.h"

std::string strip_backtrace(const std::string &s)
{
    size_t pos = s.find("\nBacktrace:");
    if (std::string::npos == pos)
        return s;
    return s.substr(0, pos);
}

class TestContentChecker: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestContentChecker);
    CPPUNIT_TEST(testCheckContent);
    CPPUNIT_TEST(testCheckContent_unknown_type);
    CPPUNIT_TEST(testCheckContent_attribute_not_existed);
    CPPUNIT_TEST(testCheckContent_main_tags_not_existed);
    CPPUNIT_TEST_SUITE_END(); 
public:
    void setUp()
    {}
    void tearDown()
    {}
    void testCheckContent()
    {   
        StringDict content;  
        content["&content-type"] = "text/xml";
        content["&post-data"] = "<request version=\"0.1\" type=\"auth\">\n"
                                    "<login>test_login</login>\n"
                                    "<pass>test_pass</pass>\n"
                                "</request>";
        ContentChecker contentChecker;
        CPPUNIT_ASSERT_EQUAL(true, contentChecker.contentIsValid(content));
        CPPUNIT_ASSERT_EQUAL(int(Auth), contentChecker.requestType());
    }
    void testCheckContent_unknown_type()
    {
        StringDict content;  
        content["&content-type"] = "json";
        content["&post-data"] = "<request version=\"0.1\" type=\"auth\">\n"
                                    "<login>test_login</login>\n"
                                    "<pass>test_pass</pass>\n"
                                "</request>";            
        ContentChecker contentChecker;
        CPPUNIT_ASSERT_EQUAL(false, contentChecker.contentIsValid(content));
        CPPUNIT_ASSERT_EQUAL(std::string("Unknown content type: json"), contentChecker.err());
    }
    void testCheckContent_attribute_not_existed()
    {
        StringDict content;  
        content["&content-type"] = "text/xml";
        content["&post-data"] = "<request version=\"0.1\" left_attribute=\"123\">\n"
                                    "<login>test_login</login>\n"
                                    "<pass>test_pass</pass>\n"
                                "</request>";            
        ContentChecker contentChecker;
        CPPUNIT_ASSERT_EQUAL(false, contentChecker.contentIsValid(content));
    }
    void testCheckContent_main_tags_not_existed()
    {
        StringDict content; 
        content["&content-type"] = "text/xml";
        content["&post-data"] = "<request version=\"0.1\" type=\"auth\">\n"
                                    "<login>test_login</login>\n"
                                "</request>";            
        ContentChecker contentChecker;
        CPPUNIT_ASSERT_EQUAL(false, contentChecker.contentIsValid(content));
        CPPUNIT_ASSERT_EQUAL(std::string("pass"),
                strip_backtrace(contentChecker.err()));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestContentChecker);

// vim:ts=4:sts=4:sw=4:et:
