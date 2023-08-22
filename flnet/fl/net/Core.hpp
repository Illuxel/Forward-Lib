<<<<<<< Updated upstream
#pragma once

#ifdef _WIN32
	#include <SDKDDKVer.h>
#endif

#include <boost/system/error_code.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

#include <thread>

#include "fl/utils/Memory.hpp"

namespace Forward::Net {
		
	namespace Core {

		namespace Asio = boost::asio;
		namespace Ip = Asio::ip;

		namespace Ssl = Asio::ssl;

		using Tcp = Asio::ip::tcp;
		using Error = boost::system::error_code;

	}
=======
#pragma once

#ifdef _WIN32
	#include <SDKDDKVer.h>
#endif

#include "fl/utils/Memory.hpp"

#include <boost/system/error_code.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/io_context.hpp>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/asio/ssl.hpp>

#include <thread>

namespace Forward::Net {
		
	// Boost core
	namespace Core { 

		using ErrorCode = boost::system::error_code;

		namespace Error = boost::asio::error;

		namespace IP {
			
			template<class T>
			using BaseEndpoint = boost::asio::ip::basic_endpoint<T>;

			using UDP = boost::asio::ip::udp;
			using UDPSocketBase = UDP::socket;

			using TCP = boost::asio::ip::tcp;
			using TCPSocketBase = TCP::socket;
		}

		namespace SSL {
			
			using Method = boost::asio::ssl::context::method;
			using FileFormat = boost::asio::ssl::context::file_format;
		}
	}
>>>>>>> Stashed changes
}