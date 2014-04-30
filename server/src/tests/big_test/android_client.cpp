#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include "android_client.h"

AndroidClient::AndroidClient(std::string _name, std::string _host, int _port): 
    name(_name), host(_host), port(_port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET; 
	raw_host = gethostbyname(host.c_str()); 
	addr.sin_addr.s_addr = ((struct in_addr *)raw_host->h_addr_list[0])->s_addr;
	addr.sin_port = htons(_port);
}

AndroidClient::~AndroidClient() 
{
	close(sock);
}

int 
AndroidClient::connect()
{
	if (::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		return false;
	return true;	
}

std::string 
AndroidClient::request_constructor(std::string method, std::string handler, std::string content)
{
    std::string query = "Host: localhost\r\n"
                        "User-Agent: Mozilla/5.0\r\n"
                        "Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3\r\n"
                        "Accept-Charset: windows-1251,utf-8;q=0.7\r\n"
                        "Connection: keep-alive\r\n";
    if (!method.compare("POST")) {
        query.insert(0, "POST " + handler + " HTTP/1.1\r\n");
        query.append("Content-Type: text/xml\r\n");
        std::stringstream ss;
        ss << "Content-Length: " << content.size() << "\r\n\r\n";
        query.append(ss.str()); 
        query.append(content);
    } else if (!method.compare("GET")) {
        query.insert(0, "GET " + handler + " HTTP/1.1\r\n");
        query.append("\r\n");
    } else 
        std::cout << "Unknown method.." << std::endl;
    return query;
}

bool
AndroidClient::test_auth()
{
    std::cout << "----------Run test_auth()----------" << std::endl;
    std::string request = request_constructor("POST", "/main", 
            "<request version='0.1' type='auth'>\r\n"
                "<login>medved</login>\r\n"
                "<pass>preved</pass>\r\n"
                "<time>12:00</time>\r\n"
            "</request>"); 
    std::cout << "Request: " << request << std::endl;
    write(sock, query.c_str(), query.size());
    std::string resp;
    char temp[1024];
    while (read(sock, temp, 1024) > 0)
        resp.append(temp);
    resp.erase(0, resp.find("<token>") + 7);
    resp = resp.substr(0, resp.find("</token>"));
    std::cout << "Response: " << resp << std::endl;
    std::cout << "----------End test_auth()----------" << std::endl;
    token = resp;
    return !resp.compare("\r") ? false : true;
}

bool
AndroidClient::test_download()
{
    return false;
}

bool
AndroidClient::test_update()
{ 
    return false;
}

bool
AndroidClient::test_check()
{ 
    return false;
}

bool
AndroidClient::test_logout()
{ 
    std::cout << "----------Run test_logout()----------" << std::endl;
    std::string query = query_constructor("POST", "/main", 
            "<request version='0.1' type='logout'>"
                "<token>" + token  + "</token>"
                "<time>12:00</time>"
            "</request>"); 
    std::cout << "Query: " << query << std::endl;
    write(sock, query.c_str(), query.size());
    std::string resp,
                temp;
    //while (read(sock, temp.c_str(), 1024) > 0)
    //    resp.append(temp);
    std::cout << "Response: " << resp << std::endl;
    std::cout << "----------End test_logout()----------" << std::endl;
    return !resp.compare("") ? false : true;
}  

// vim:ts=4:sts=4:sw=4:et:
