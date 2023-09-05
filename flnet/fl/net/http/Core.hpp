#pragma once

#include <set>
#include <unordered_map>

#include <filesystem>

#include <shared_mutex>

#include "fl/net/Core.hpp"

#include "fl/utils/MimeType.hpp"
#include "fl/utils/StringArg.hpp"

#include <boost/json.hpp>
namespace json = boost::json;

#include <boost/beast/version.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

namespace Forward::Web {
	
	namespace Core {

		using namespace Forward::Net::Core; // To use all core feautures of Net in Web 

		namespace Http {
			
			using Method = boost::beast::http::verb;
		}
	} 

}