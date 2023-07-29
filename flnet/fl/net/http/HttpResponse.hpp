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

        constexpr HttpResponseWrapper(http::response<Body>&& response)
            : response_(std::move(response)){}
        constexpr HttpResponseWrapper(http::response<Body> const& response)
            : response_(response) {}

        constexpr auto const& base() const&
        {
            return response_;
        }
        constexpr auto& base() &
        {
            return response_;
        }
        constexpr auto&& base() &&
        {
            return std::move(response_);
        }
        constexpr auto const&& base() const&&
        {
            return std::move(response_);
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

        constexpr operator http::response<Body> const&() const&
        {
            return response_;
        }
        constexpr operator http::response<Body>&() &
        {
            return response_;
        }
        constexpr operator http::response<Body>&&() &&
        {
            return std::move(response_);
        }
        constexpr operator http::response<Body>&&() const&&
        {
            return std::move(response_);
        }

        constexpr operator http::message_generator()
        {
            return std::move(response_);
        }
    };

    using HttpResponse = HttpResponseWrapper<http::string_body>;
    using HttpResponseFile = HttpResponseWrapper<http::file_body>;
    using HttpResponseEmpty = HttpResponseWrapper<http::empty_body>;
}