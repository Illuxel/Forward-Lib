#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward::Net {

	class TcpSocket 
	{
	private:
		Core::IOContext io_context_;

		Core::TcpSocketBase socket_;
		Core::TcpResolver resolver_;

	public:
		TcpSocket(Endpoint const& address);
		//TcpSocket(Core::TcpSocketBase socket);

		virtual ~TcpSocket();

	protected:
		virtual void OnReadData(Core::MutableBuffer const&);

	};
}