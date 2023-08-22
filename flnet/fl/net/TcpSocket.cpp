#include "fl/net/TcpSocket.hpp"

namespace Forward::Net {

	TcpSocket::TcpSocket(Endpoint const& address)
		: socket_(io_context_)
		, resolver_()
	{
	}
	TcpSocket::TcpSocket(Core::Tcp::socket&& socket)
	{
	}

	TcpSocket::~TcpSocket()
	{

	}
}