//
// Created by sayan on 12.07.2022.
//

#ifndef BOOST_SERVER_H
#define BOOST_SERVER_H

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#include "session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

class server
{
public:
	server(io::io_context &io_context, std::uint16_t port);

	void async_accept();

	void post(std::string const &message);

private:
	io::io_context &io_context;
	tcp::acceptor acceptor;
	std::optional<tcp::socket> socket;
	std::unordered_set<std::shared_ptr<session>> clients;
};


#endif //BOOST_SERVER_H
