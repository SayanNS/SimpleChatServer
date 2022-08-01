#include <boost/asio.hpp>
#include <optional>
#include "server.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

int main()
{
	io::io_context io_context;
	server srv(io_context, 15001);
	srv.async_accept();
	io_context.run();
	return 0;
}
