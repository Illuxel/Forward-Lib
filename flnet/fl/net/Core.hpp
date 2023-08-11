#pragma once

#ifdef _WIN32
	#include <SDKDDKVer.h>
#endif

#include <optional>

#include <boost/system/error_code.hpp>

namespace sys = boost::system;

#include <boost/asio/strand.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

namespace net = boost::asio;
namespace ssl = net::ssl;

using tcp = boost::asio::ip::tcp;
