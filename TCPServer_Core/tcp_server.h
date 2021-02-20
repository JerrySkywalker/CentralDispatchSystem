#pragma once

#include <memory>
#include <system_error>

#define BOOST_ASIO_NO_DEPRECATED
#define BOOST_ASIO_DISABLE_STD_CHRONO
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING


#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

constexpr int buffer_size = 1024;

class session : public std::enable_shared_from_this<session> {
public:
	session(boost::asio::ip::tcp::socket socket) :
		socket_(std::move(socket))
	{
    }

    void start() {
        do_read();
    }

private:

    boost::asio::ip::tcp::socket    socket_;
    std::array<char, buffer_size>   buffer_;
	
    void do_read();
    void do_write(std::size_t length);
};

class server {
public:
    server(boost::asio::io_context& io_context, std::uint16_t port)
        : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        do_accept();
    }

private:

    boost::asio::ip::tcp::acceptor          acceptor_;
	
    void do_accept();
};