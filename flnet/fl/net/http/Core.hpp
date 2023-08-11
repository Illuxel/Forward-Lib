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

namespace beast = boost::beast;
namespace http = beast::http;