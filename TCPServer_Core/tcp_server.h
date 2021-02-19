#pragma once

#include <memory>
#include <system_error>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/execution/start.hpp>
#include <boost/asio/ip/tcp.hpp>

#define BOOST_ASIO_DISABLE_STD_CHRONO
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

using namespace boost::asio;

class tcp_server
{
	typedef tcp_server							this_type;
	typedef ip::tcp::acceptor					acceptor_type;
	typedef ip::tcp::endpoint					endpoint_type;
	typedef ip::tcp::socket						socket_type;
	typedef boost::shared_ptr<socket_type>		sock_ptr;
	typedef std::vector<char>					buffer_type;

private:
	io_service									m_io;
	acceptor_type								m_acceptor;
	buffer_type									m_buffer_read;
	buffer_type									m_buffer_write;

public:
	tcp_server() :
		m_buffer_read(100, 0),
		m_buffer_write(100, 0),
		m_acceptor(m_io, endpoint_type(ip::tcp::v4(), 6688))
	{
		accept();
	}


	void run()
	{
		m_io.run();
	}

private:
	void accept();
	void accept_handler(const boost::system::error_code& ec, sock_ptr sock);
	void write_handler(const boost::system::error_code&, std::size_t n);

	void read_handler(const boost::system::error_code& ec, sock_ptr sock);
};
