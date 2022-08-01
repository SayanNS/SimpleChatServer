//
// Created by sayan on 12.07.2022.
//

#include "session.h"

session::session(tcp::socket &&socket)
	: socket(std::move(socket))
{
}

void session::start(message_handler &&on_message, error_handler &&on_error)
{
	this->on_message = std::move(on_message);
	this->on_error = std::move(on_error);
	async_read();
}

void session::post(std::string const &message)
{
	bool idle = outgoing.empty();
	outgoing.push(message);

	if (idle) {
		async_write();
	}
}

void session::async_read()
{
	io::async_read_until(socket, streambuf, "\n", [capture0 = shared_from_this()](auto &&PH1, auto &&PH2) {
		capture0->on_read(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
	});
}

void session::on_read(error_code error, std::size_t bytes_transferred)
{
	if (!error) {
		std::stringstream message;
		message << socket.remote_endpoint(error) << ": " << std::istream(&streambuf).rdbuf();
		streambuf.consume(bytes_transferred);
		on_message(message.str());
		async_read();
	} else {
		socket.close(error);
		on_error();
	}
}

void session::async_write()
{
	io::async_write(socket, io::buffer(outgoing.front()), [capture0 = shared_from_this()](auto &&PH1, auto &&PH2) {
		capture0->on_write(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
	});
}

void session::on_write(error_code error, std::size_t bytes_transferred)
{
	if (!error) {
		outgoing.pop();

		if (!outgoing.empty()) {
			async_write();
		}
	} else {
		socket.close(error);
		on_error();
	}
}
