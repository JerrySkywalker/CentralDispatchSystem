#include <iostream>
#include "tcp_server.h"

int main()
{
	std::cout << "CASC Contest 2021.2" << std::endl;
	std::cout << "Sample: TCP Server" << std::endl;
	
	tcp_server tcp_svr;

	tcp_svr.run();
}
