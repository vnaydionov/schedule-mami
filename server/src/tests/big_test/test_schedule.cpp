#include <iostream>
#include "android_client.h"

int main(int argc, char *argv[])
{
	std::cout << "----------Start tests----------" << std::endl;
	AndroidClient clt("Android", "localhost", 9090);
	if (clt.connect() == -1) {
		std::cout << "Error: connect to host.." << std::endl;
		return -1;
	}
	std::cout << "Connection was successful!" << std::endl;
    if (clt.test_auth() && clt.test_logout())
	    std::cout << "All tests were successful!" << std::endl;
    else 
	    std::cout << "An error somewhere.." << std::endl;
	std::cout << "----------End tests----------" << std::endl;
	return 0;
}
