#pragma once

#include <array>
#include <string>

#define BOOST_ASIO_NO_DEPRECATED
#define BOOST_ASIO_DISABLE_STD_CHRONO
//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <boost/asio.hpp>

// Use async_resolve() or not.
#define RESOLVE_ASYNC 1

// Only resolve IPv4.
#define RESOLVE_IPV4_ONLY 1

constexpr int buffer_size = 1024;

class client {
public:
	
    client(boost::asio::io_context& io_context,
        const std::string& host, const std::string& port);

    void shutdown();

private:
	
    boost::asio::ip::tcp::socket            socket_;
#if RESOLVE_ASYNC
    boost::asio::ip::tcp::resolver          resolver_;
#endif

    char                                    cin_buf_[buffer_size];
    std::array<char, buffer_size>           buf_;

	
#if RESOLVE_ASYNC
    void on_resolve(boost::system::error_code ec,
                   boost::asio::ip::tcp::resolver::results_type endpoints);
#endif  // RESOLVE_ASYNC
	
    void connect_handler(boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint);

    void do_write();
	
    void write_handler(boost::system::error_code ec, std::size_t length);
    void read_handler(boost::system::error_code ec, std::size_t length);

};
