#include <iostream>
#include <exception>
#include "tcp_server.h"

int main(const int argc, char* argv[])
try
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}
	
	std::cout << "CASC Contest 2021.2" << std::endl;
	std::cout << "Sample: TCP Server" << std::endl;

	std::cout << std::endl<<"Running on localhost:" << argv[1] << std::endl;
	
	const std::uint16_t port = std::atoi(argv[1]);  // NOLINT(clang-diagnostic-implicit-int-conversion, cert-err34-c)
	
	boost::asio::io_context ioc;

	server server_demo{ ioc, port };

	ioc.run();

	return 0;
}
catch (std::exception& e)
{
	std::cout << "Standard exception: " << e.what() << std::endl;
}