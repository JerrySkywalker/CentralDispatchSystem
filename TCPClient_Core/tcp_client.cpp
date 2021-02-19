#include "tcp_client.h"

#include <iostream>
#include <boost/bind.hpp>




/*Deprecated with old boost::asio*/

//tcp_client::tcp_client(const char* address, const int port):
//	m_endpoint(address_type::from_string(address), port),  // NOLINT(clang-diagnostic-implicit-int-conversion)
//	m_buffer_read(100, 0)
//{
//	start();
//}
//
//void tcp_client::start()
//{
//	sock_ptr sock(new socket_type(m_io));
//	sock->async_connect(m_endpoint,
//		[this, sock](const boost::system::error_code& ec)
//		{
//			if (ec)
//				return;
//
//			sock->async_read_some(buffer(m_buffer_read),
//				[this, sock](const boost::system::error_code& ec, std::size_t)
//				{
//					read_handler(ec, sock);
//				}
//			);
//		});
//}
//
//void tcp_client::conn_handler(const boost::system::error_code& ec, sock_ptr sock)
//{
//	if (ec)
//		return;
//
//	std::cout << "receive from " << sock->remote_endpoint().address();
//
//	sock->async_read_some(buffer(m_buffer_read),
//		boost::bind(&tcp_client::read_handler,this,boost::asio::placeholders::error,
//			sock)
//	);
//	
//}
//
//void tcp_client::read_handler(const boost::system::error_code& ec, sock_ptr sock)
//{
//	if (ec)
//		return;
//
//	std::cout << &m_buffer_read[0] << std::endl;
//
//	sock->async_read_some(buffer(m_buffer_read),
//		boost::bind(&tcp_client::read_handler, this, boost::asio::placeholders::error,
//			sock)
//	);
//
//	
//}

Client::Client(boost::asio::io_context& io_context,
    const std::string& host, const std::string& port)
#if RESOLVE_ASYNC
    : socket_(io_context), resolver_(io_context) {
#else
    : socket_(io_context) {
#endif

#if RESOLVE_ASYNC

    resolver_.async_resolve(boost::asio::ip::tcp::v4(), host, port,
                            std::bind(&Client::on_resolve, this,
                                      std::placeholders::_1,
                                      std::placeholders::_2));

#else

    // If you don't specify tcp::v4() as the first parameter (protocol) of
    // resolve(), the result will have two endpoints, one for v6, one for
    // v4. The first v6 endpoint will fail to connect.

    tcp::resolver resolver(io_context);

#if RESOLVE_IPV4_ONLY

    auto endpoints = resolver.resolve(tcp::v4(), host, port);
    // 127.0.0.1:2017, v4

#else

    auto endpoints = resolver.resolve(host, port);
    // [::1]:2017, v6
    // 127.0.0.1:2017, v4

#endif  // RESOLVE_IPV4_ONLY

    utility::PrintEndpoints(std::cout, endpoints);

    // ConnectHandler: void(boost::system::error_code, tcp::endpoint)
    boost::asio::async_connect(socket_, endpoints,
        std::bind(&Client::OnConnect, this,
            std::placeholders::_1,
            std::placeholders::_2));

#endif  // RESOLVE_ASYNC
}

#if RESOLVE_ASYNC

void Client::on_resolve(boost::system::error_code ec,
                       boost::asio::ip::tcp::resolver::results_type endpoints) {
    if (ec) {
        std::cerr << "Resolve: " << ec.message() << std::endl;
    }
    else {
        // ConnectHandler: void(boost::system::error_code, tcp::endpoint)
        boost::asio::async_connect(socket_, endpoints,
            std::bind(&Client::on_connect, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
}

#endif  // RESOLVE_ASYNC

void Client::on_connect(boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
    if (ec) {
        std::cout << "Connect failed: " << ec.message() << std::endl;
        socket_.close();
    }
    else {
        do_write();
    }
}

void Client::do_write() {
    std::size_t len = 0;
    do {
        std::cout << "Enter message: ";
        std::cin.getline(cin_buf_, BUF_SIZE);
        len = strlen(cin_buf_);
    } while (len == 0);

    // TODO: Second parameter
    // WriteHandler: void (boost::system::error_code, std::size_t)
    boost::asio::async_write(socket_,
        boost::asio::buffer(cin_buf_, len),
        std::bind(&Client::on_write, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Client::on_write(boost::system::error_code ec, std::size_t length) {
    boost::ignore_unused(length);

    if (!ec) {
        std::cout << "Reply is: ";

        socket_.async_read_some(boost::asio::buffer(buf_),
            std::bind(&Client::on_read, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
}

void Client::on_read(boost::system::error_code ec, std::size_t length) {
    if (!ec) {
        std::cout.write(buf_.data(), length);
        std::cout << std::endl;
    }

    // Pause, please press any key to continue.
    getchar();

    std::cout << "Shutdown socket..." << std::endl;

    // Initiate graceful connection closure.
    // Socket close VS. shutdown:
    //   https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket
    boost::system::error_code ec2;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec2);

    // shutdown() with `shutdown_both` or `shutdown_send` signals EOF to the peer.
    // It means that the peer will read an EOF.

    if (ec2) {
        std::cerr << "Socket shutdown error: " << ec2.message() << std::endl;
        ec2.clear();
        // Don't return, try to close the socket anywhere.
    }

    // Pause, please press any key to continue.
    getchar();

    std::cout << "Close socket..." << std::endl;

    socket_.close(ec2);

    if (ec2) {
        std::cerr << "Socket close error: " << ec2.message() << std::endl;
    }

    // Optionally, continue to write.
    // DoWrite();
}