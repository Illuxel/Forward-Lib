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
}