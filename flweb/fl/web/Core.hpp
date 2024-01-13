#pragma once

#include "fl/net/Core.hpp"

#include "fl/core/MimeType.hpp"
#include "fl/core/StringArg.hpp"

#include <set>
#include <unordered_map>
#include <filesystem>

#include <shared_mutex>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <boost/json.hpp>

namespace Forward::Web {

	using HttpMethod = boost::beast::http::verb;
	using HttpField = boost::beast::http::field;
	using HttpStatus = boost::beast::http::status;

	namespace Core {

		using namespace Forward::Net::Core;

		using AnyIOContext = boost::asio::any_io_executor;

		using FlatBuffer = boost::beast::flat_buffer;
		using MultiBuffer = boost::beast::multi_buffer;

		template<uint64_t Size>
		using StaticBuffer = boost::beast::static_buffer<Size>;

		using EmptyBody = boost::beast::http::empty_body;
		using StringBody = boost::beast::http::string_body;
		using FileBody = boost::beast::http::file_body;

		using MessageGenerator = boost::beast::http::message_generator;

		template<class Body, class Fields = boost::beast::http::fields>
		using HttpBaseRequest = boost::beast::http::request<Body>;

		template<class Body, class Fields = boost::beast::http::fields>
		using HttpBaseResponse = boost::beast::http::response<Body>;

		using TcpStream = boost::beast::tcp_stream;

		namespace SSL {

			using namespace Forward::Net::Core::SSL;

			template<class Type>
			using Stream = boost::beast::ssl_stream<Type>;

			template<class Type>
			using FlatStream = boost::beast::flat_stream<Type>;

		}

		namespace Beast {

			using namespace boost::beast;
			using namespace boost::beast::http;

		} // Beast

	} // Core

} // Forward::Web