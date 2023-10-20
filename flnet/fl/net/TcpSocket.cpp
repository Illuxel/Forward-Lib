#include "fl/net/TcpSocket.hpp"

namespace Forward::Net {

	TcpSocket::TcpSocket(Endpoint const& address)
		: socket_(io_context_, address.Protocol())
		, resolver_(io_context_)
	{
	}
	//TcpSocket::TcpSocket(Core::TcpSocketBase socket)
	//{
	//}

	TcpSocket::~TcpSocket()
	{

	}
}