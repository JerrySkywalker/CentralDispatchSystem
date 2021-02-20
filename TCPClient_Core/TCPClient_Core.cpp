// ReSharper disable once CppInconsistentNaming
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(clang-diagnostic-reserved-id-macro)

#include <iostream>
#include <string>

#include "tcp_client.h"

int main(int argc, char* argv[])
try
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;
		return 1;
	}
	
	std::cout << "CASC Contest 2021.2" << std::endl;
	std::cout << "Sample: TCP Client" << std::endl;

	const std::string host = argv[1];
	const std::string port = argv[2];

	boost::asio::io_context ioc;

	client client{ ioc, host, port };
	
	ioc.run();
	
}
catch (std::exception& e)
{
	std::cout << "Standard exception: " << e.what() << std::endl;
}