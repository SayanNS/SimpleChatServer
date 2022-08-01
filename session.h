//
// Created by sayan on 12.07.2022.
//

#ifndef BOOST_SESSION_H
#define BOOST_SESSION_H

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

class session : public std::enable_shared_from_this<session>
{
public:
	session(boost::asio::ip::tcp::socket &&socket);

	void start(message_handler &&on_message, error_handler &&on_error);

	void post(std::string const &message);

private:
	void async_read();

	void on_read(error_code error, std::size_t bytes_transferred);

	void async_write();

	void on_write(error_code error, std::size_t bytes_transferred);

	tcp::socket socket;
	io::streambuf streambuf;
	std::queue<std::string> outgoing;
	message_handler on_message;
	error_handler on_error;
};


#endif //BOOST_SESSION_H
