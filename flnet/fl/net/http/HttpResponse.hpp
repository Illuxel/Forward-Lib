#pragma once

#include "fl/net/http/Core.hpp"

namespace Forward {

    template<class Body>
    class HttpResponseWrapper
    {
    private:
        http::response<Body> response_;

    public:
        HttpResponseWrapper() 
            : response_{} {}

        HttpResponseWrapper(http::status status, int version)
            : response_(status, version) {}

        HttpResponseWrapper(http::response<Body>&& response)
            : response_(std::move(response)){}
        HttpResponseWrapper(http::response<Body> const& response) 
            : response_(response) {}

        auto& Base() & 
        {
            return response_;
        }
        auto&& Base() &&
        {
            return std::move(response_);
        }
        auto const& Base() const &
        {
            return response_;
        }

        void Clear() 
        {
            request_ = {};
        }

        static HttpResponseWrapper<http::string_body> Message(
            http::response<http::string_body>&& res, 
            std::string const& msg) 
        {
            res.set(http::field::content_type, MimeType::FromString("txt").GetMimeName());
            res.body() = std::move(msg);
            return std::move(res);
        } 
        static HttpResponseWrapper<http::string_body> Message(
            http::response<http::string_body>&& res, 
            boost::json::value const& msg) 
        {
            res.set(http::field::content_type, MimeType::FromString("json").GetMimeName());
            res.body() = std::move(boost::json::serialize(msg));
            return std::move(res);
        } 

        operator http::response<Body>&() & 
        {
            return &response_;
        }
        operator http::response<Body>&&() &&
        {
            return std::move(response_);
        }
        operator http::response<Body>() const &
        {
            return response_;
        }

        operator http::message_generator() 
        {
            return std::move(response_);
        }
    };

    using HttpResponse = HttpResponseWrapper<http::string_body>;
    using HttpResponseFile = HttpResponseWrapper<http::file_body>;
    using HttpResponseEmpty = HttpResponseWrapper<http::empty_body>;
}