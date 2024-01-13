#pragma once

#include "fl/core/Memory.hpp"

#include <thread>

#ifdef _WIN32
	#include <SDKDDKVer.h>
#endif

#include <boost/system/error_code.hpp>

#include <boost/asio/version.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

namespace Forward::Net {

	namespace Core { 

		using ErrorCode = boost::system::error_code;

		namespace Error = boost::asio::error;

		using IOContext = boost::asio::io_context;

		using MutableBuffer = boost::asio::mutable_buffer;

		template<class T>
		using BaseEndpoint = boost::asio::ip::basic_endpoint<T>;
		using IpAddress = boost::asio::ip::address;
		using IpAddressV4 = boost::asio::ip::address_v4;
		using IpAddressV6 = boost::asio::ip::address_v6;

		using Udp = boost::asio::ip::udp;
		using UdpSocketBase = Udp::socket;
		using UdpResolver = Udp::resolver;	

		using Tcp = boost::asio::ip::tcp;
		using TcpSocketBase = Tcp::socket;
		using TcpResolver = Tcp::resolver;
		using TcpAcceptor = Tcp::acceptor;

		namespace SSL {
			using Context = boost::asio::ssl::context;

			using StreamBase = boost::asio::ssl::stream_base;

			using Method = boost::asio::ssl::context::method;
			using FileFormat = boost::asio::ssl::context::file_format;
		}

		namespace Asio {
			using namespace boost::asio;
			using namespace boost::asio::ip;
		}
	}
}