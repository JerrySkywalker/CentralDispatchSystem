#include "pch.h"

#include "tcp_client.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

static std::string GetTimeStamp_Now(void)
{
	auto time_now = boost::chrono::system_clock::to_time_t(boost::chrono::system_clock::now());
	auto time_now_str = std::string(ctime(&time_now));

	return time_now_str.replace(time_now_str.find("\n"), 1, "");
}

client::client(boost::asio::io_context& io_context,  // NOLINT(cppcoreguidelines-pro-type-member-init)
	const std::string& host,
	const std::string& port)
#if RESOLVE_ASYNC
	: socket_(io_context), resolver_(io_context) {
#else
	: socket_(io_context) {
#endif

#if RESOLVE_ASYNC

	memset(cin_buf_, 0, sizeof(cin_buf_));

	resolver_.async_resolve(boost::asio::ip::tcp::v4(), host, port,
		std::bind(&client::on_resolve, this,
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

void client::shutdown()
{
	std::cout << "Shutdown socket..." << std::endl;

	// Initiate graceful connection closure.
	// Socket close VS. shutdown:
	//   https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket
	boost::system::error_code ec;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

	// shutdown() with `shutdown_both` or `shutdown_send` signals EOF to the peer.
	// It means that the peer will read an EOF.

	if (ec) {
		std::cerr << "Socket shutdown error: " << ec.message() << std::endl;
		ec.clear();
		// Don't return, try to close the socket anywhere.
	}

	// Pause, please press any key to continue.
	getchar();

	std::cout << "Close socket..." << std::endl;

	socket_.close(ec);

	if (ec) {
		std::cerr << "Socket close error: " << ec.message() << std::endl;
	}
}

#if RESOLVE_ASYNC

void client::on_resolve(boost::system::error_code ec,
	boost::asio::ip::tcp::resolver::results_type endpoints) {
	if (ec) {
		std::cerr << "[Error] Resolve error: " << ec.message() << std::endl;
	}
	else {
		// ConnectHandler: void(boost::system::error_code, tcp::endpoint)
		boost::asio::async_connect(socket_, endpoints,
			std::bind(&client::connect_handler, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
}

#endif  // RESOLVE_ASYNC

void client::connect_handler(boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
	boost::ignore_unused<boost::asio::ip::tcp::endpoint>(endpoint);

	if (ec)
	{
		std::cout << "[Error] Connect failed: " << ec.message() << std::endl;
		socket_.close();
	}
	else {
		do_write();
	}
}

void client::do_write()
{
	std::size_t len = 0;

	auto time_now = boost::chrono::system_clock::to_time_t(boost::chrono::system_clock::now());
	auto time_now_str = std::string(ctime(&time_now));

	std::cout << std::endl << "[" + time_now_str.replace(time_now_str.find("\n"), 1, "") + "] "
		<< "To " << socket_.remote_endpoint().address() << ":" << socket_.remote_endpoint().port();
	std::cout << std::endl;

	std::cout << "Enter message: ";
	do {
		std::cin.getline(cin_buf_, buffer_size);
		len = strlen(cin_buf_);
	} while (len == 0);

	boost::asio::async_write(socket_,
		boost::asio::buffer(cin_buf_, len),
		std::bind(&client::write_handler, this,
			std::placeholders::_1,
			std::placeholders::_2));
}

void client::write_handler(boost::system::error_code ec, std::size_t length) {
	boost::ignore_unused(length);

	if (!ec) {
		socket_.async_read_some(boost::asio::buffer(buf_),
			std::bind(&client::read_handler, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
	else
	{
		std::cerr << "[Error] Async write error: " << ec.message() << std::endl;
	}
}

void client::read_handler(boost::system::error_code ec, std::size_t length) {
	if (!ec) {
		auto time_now = boost::chrono::system_clock::to_time_t(boost::chrono::system_clock::now());
		auto time_now_str = std::string(ctime(&time_now));

		std::cout << std::endl << "[" + time_now_str.replace(time_now_str.find("\n"), 1, "") + "] "
			<< "From " << socket_.remote_endpoint().address() << ":" << socket_.remote_endpoint().port();
		std::cout << std::endl;

		std::cout << "Reply is: ";

		std::cout.write(buf_.data(), length);
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "[Error] Async read error: " << ec.message() << std::endl;
	}

	do_write();
}