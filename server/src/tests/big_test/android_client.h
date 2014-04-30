#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class AndroidClient {
	private:
		struct sockaddr_in addr;
		struct hostent* raw_host;
		std::string name;
		std::string host;
        std::string token;
        int sock;
		int port;
        std::string query_constructor(std::string method, 
                std::string handler, std::string content="");

	public:
		AndroidClient(std::string _name, std::string _host, int _port);
		AndroidClient();
		~AndroidClient();
		int connect();
        bool test_auth();
        bool test_download();
        bool test_update();
        bool test_check();
        bool test_logout();
};

// vim:ts=4:sts=4:sw=4:et:
