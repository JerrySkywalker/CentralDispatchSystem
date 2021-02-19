// ReSharper disable once CppInconsistentNaming
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(clang-diagnostic-reserved-id-macro)

#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>

#include "tcp_client.h"
#include "avg.h"

int main()
{
	std::cout << "CASC Contest 2021.2" << std::endl;
	std::cout << "Sample: TCP Client" << std::endl;
	std::cout << std::string(20, '=') << std::endl << std::endl;

	int address[5] = { 0 };

	while (true)
	{
		std::cout << "Target:" << std::endl;
		scanf_s("%d.%d.%d.%d:%d", &address[0], &address[1], &address[2], &address[3], &address[4]);

		try {
			for (int i = 0; i <= 3; i++)
			{
				if (address[i] < 0 || address[i]>255)
					throw "Invalid IP!";  // NOLINT(hicpp-exception-baseclass)
			}
			if (address[4] < 0 || address[4]>9999)
				throw "Invalid IP!";	// NOLINT(hicpp-exception-baseclass)

			break;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl << std::endl;
		}
	}

	avg yq01(
		std::to_string(address[0]) + "." + std::to_string(address[1]) + "." + std::to_string(address[2]) + "." +
		std::to_string(address[3]),
		address[4]);

	std::cout << std::endl << "TCP Client Start.Waiting for User Input ..." << std::endl << std::endl;

	tcp_client tcp_cl(yq01.m_ip.data(), yq01.m_port);

	tcp_cl.run();

	std::string user_input;

	while (true)
	{
		auto now = chrono::system_clock::now();
		auto time_now = chrono::system_clock::to_time_t(now);

		std::cout << std::endl << "hitsic # " << std::string(ctime(&time_now)) << ">>";
		std::cin >> user_input;
	}
}