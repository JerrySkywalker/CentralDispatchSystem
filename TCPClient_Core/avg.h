#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class avg
{
public:
	std::string			m_ip;
	int					m_port;
	std::vector<char>	m_command;

	avg() :
		m_ip("127.0.0.1"),
		m_port(6688)
	{
		std::cout << "Test form local server." << std::endl;
	}

	avg(std::string ip,const int port ) :
		m_ip(std::move(ip)),
		m_port(port)
	{
		std::cout << "Target Selected: " << std::endl;
		std::cout << "	- IP: " << m_ip<<std::endl;
		std::cout << "	- Port: " << m_port << std::endl;
		
	}

	~avg();
};
