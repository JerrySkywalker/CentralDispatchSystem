#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

//#define BOOST_ASIO_NO_DEPRECATED
#define BOOST_ASIO_DISABLE_STD_CHRONO
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <boost/asio.hpp>
#include <boost/core/ignore_unused.hpp>

// Use async_resolve() or not.
#define RESOLVE_ASYNC 1

// Only resolve IPv4.
#define RESOLVE_IPV4_ONLY 1

class Client {
public:
	
    Client(boost::asio::io_context& io_context,
        const std::string& host, const std::string& port);

private:
	
    boost::asio::ip::tcp::socket            socket_;
#if RESOLVE_ASYNC
    boost::asio::ip::tcp::resolver          resolver_;
#endif

    enum { BUF_SIZE = 1024 };
    char                                    cin_buf_[BUF_SIZE];
    std::vector<char>                       buf_;

	
#if RESOLVE_ASYNC
    void on_resolve(boost::system::error_code ec,
                   boost::asio::ip::tcp::resolver::results_type endpoints);
#endif  // RESOLVE_ASYNC
	
    void on_connect(boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint);

    void do_write();
	
    void on_write(boost::system::error_code ec, std::size_t length);
    void on_read(boost::system::error_code ec, std::size_t length);
	
};


/*Deprecated with old boost::asio*/

//#include <boost/asio.hpp>
//#include <boost/asio/ip/tcp.hpp>
//
//using namespace boost::asio;  // NOLINT(clang-diagnostic-header-hygiene)
//
//class tcp_client
//{
//	typedef tcp_client						this_type;
//	typedef ip::tcp::endpoint				endpoint_type;
//	typedef ip::address						address_type;
//	typedef ip::tcp::socket					socket_type;
//	typedef std::shared_ptr<socket_type>	sock_ptr;
//	typedef std::vector<char>				buffer_type;
//	
//private:
//	io_service								m_io;
//	endpoint_type							m_endpoint;
//	buffer_type								m_buffer_read;
//	buffer_type								m_buffer_write;
//
//public:
//	tcp_client() :
//		m_endpoint(address_type::from_string("127.0.0.1"), 6688),
//		m_buffer_read(100, 0)
//	{
//		start();
//	}
//
//	tcp_client(const char* address, const int port);
//
//	void run()
//	{
//		m_io.run();
//	}
//
//	void start();
//	
//	void conn_handler(const boost::system::error_code& ec, sock_ptr sock);
//	void read_handler(const boost::system::error_code& ec, sock_ptr sock);
//
//};
