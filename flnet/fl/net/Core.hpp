#pragma once

#include "fl/utils/Memory.hpp"

#include <thread>

#ifdef _WIN32
	#include <SDKDDKVer.h>
#endif

#include <boost/system/error_code.hpp>

<<<<<<< Updated upstream
=======
#include <boost/asio/io_context.hpp>
>>>>>>> Stashed changes
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/version.hpp>
<<<<<<< Updated upstream
=======

#include <thread>
>>>>>>> Stashed changes

namespace Forward::Net {

	namespace Core { 

		using ErrorCode = boost::system::error_code;

		namespace Error = boost::asio::error;

		using IOContext = boost::asio::io_context;

		using MutableBuffer = boost::asio::mutable_buffer;

<<<<<<< Updated upstream
=======
		template<class T>
		using BaseEndpoint = boost::asio::ip::basic_endpoint<T>;

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream

		static IpAddress MakeAddress(std::string_view str, ErrorCode& ec) 
		{
			return boost::asio::ip::make_address(str, ec);
		}

		namespace SSL {

=======

		namespace SSL {
>>>>>>> Stashed changes
			using Context = boost::asio::ssl::context;

			using StreamBase = boost::asio::ssl::stream_base;

			using Method = boost::asio::ssl::context::method;
			using FileFormat = boost::asio::ssl::context::file_format;
		}

		namespace Asio {
<<<<<<< Updated upstream

			using namespace boost::asio;
=======
			using namespace boost::asio;
			using namespace boost::asio::ip;
>>>>>>> Stashed changes
		}
	}
}