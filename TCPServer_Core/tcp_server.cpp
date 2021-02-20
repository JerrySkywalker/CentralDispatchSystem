// ReSharper disable once CppInconsistentNaming
#define _CRT_SECURE_NO_WARNINGS
#include "tcp_server.h"

#include <iostream>
#include <string>

void session::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some(
		boost::asio::buffer(buffer_),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {

				auto time_now = boost::chrono::system_clock::to_time_t(boost::chrono::system_clock::now());
				auto time_now_str = std::string(ctime(&time_now));
				
				std::cout << std::endl <<"[" + time_now_str.replace(time_now_str.find("\n"),1,"") + "] "
					<< "From "<< socket_.remote_endpoint().address()<<":"<<socket_.remote_endpoint().port();
				std::cout << std::endl;
				std::cout.write(buffer_.data(), length);
				std::cout<<std::endl;
				
				do_write(length);
			}
            else
            {
	            std::cerr << ec.message() << std::endl;
            }
		});

}

void session::do_write(std::size_t length)
{
	auto self(shared_from_this());

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(buffer_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                do_read();
            }
        });

}

void server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<session>(std::move(socket))->start();
            }
            do_accept();
        });
}
