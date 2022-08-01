//
// Created by sayan on 12.07.2022.
//

#include "server.h"

server::server(boost::asio::io_context &io_context, std::uint16_t port)
	: io_context(io_context)
	, acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
}

void server::async_accept()
{
	socket.emplace(io_context);

	acceptor.async_accept(*socket, [&] (error_code)
	{
		auto client = std::make_shared<session>(std::move(*socket));
		client->post("Welcome to chat\n\r");
		post("We have a newcomer\n\r");

		clients.insert(client);

		client->start
		(
				[this] (auto && PH1) { post(std::forward<decltype(PH1)>(PH1)); },
				[&, weak = std::weak_ptr(client)]
				{
					if (auto shared = weak.lock(); shared && clients.erase(shared)) {
						post("We are one less\n\r");
					}
				}
		);

		async_accept();
	});
}

void server::post(const std::string &message)
{
	for (auto &client : clients) {
		client->post(message);
	}
}
