#include "tcp_server.h"

#include <iostream>

void tcp_server::accept()
{
	sock_ptr sock(new socket_type(m_io));
	
	m_acceptor.async_accept(*sock,
		[this, sock](const boost::system::error_code& ec)
		{
			if (ec)
			{
				return;
			}

			sock->async_send(
				buffer("hello asio"),
				[](const boost::system::error_code& ec, std::size_t)
				{
					std::cout << "send msg complete." << std::endl;   }
			);

			sock->async_read_some(buffer(m_buffer_read),
				boost::bind(&tcp_server::read_handler, this, boost::asio::placeholders::error,
					sock)
			);
			
			accept();
		}
	);
}

void tcp_server::accept_handler(const boost::system::error_code& ec, sock_ptr sock)
{
	if (ec)
	{
		return;
	}

	std::cout << "client:";
	std::cout << sock->remote_endpoint().address() << std::endl;
	sock->async_write_some(buffer("hello asio"),
		bind(&this_type::write_handler, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	accept();
}

void tcp_server::write_handler(const boost::system::error_code&, std::size_t n)
{
	std::cout << "send msg " << n << std::endl;
}

void tcp_server::read_handler(const boost::system::error_code& ec, sock_ptr sock)
{
	if (ec)
		return;

	std::cout << &m_buffer_read[0] << std::endl;
}