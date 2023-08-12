#pragma once

#include "fl/net/http/Core.hpp"

namespace Forward::Web {

    template<class Body>
    class HttpResponseWrapper
    {
    private:
        Http::response<Body> response_;

    public:
        HttpResponseWrapper() 
            : response_{} {}

        HttpResponseWrapper(Http::status status, int version)
            : response_(status, version) {}

        constexpr HttpResponseWrapper(Http::response<Body>&& response)
            : response_(std::move(response)){}
        constexpr HttpResponseWrapper(Http::response<Body> const& response)
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

        static HttpResponseWrapper<Http::string_body> Message(
            Http::response<Http::string_body>&& res,
            std::string const& msg) 
        {
            res.set(http::field::content_type, MimeType::FromString("txt").GetMimeName());
            res.body() = std::move(msg);
            return std::move(res);
        } 
        static HttpResponseWrapper<Http::string_body> Message(
            Http::response<Http::string_body>&& res,
            boost::json::value const& msg) 
        {
            res.set(http::field::content_type, MimeType::FromString("json").GetMimeName());
            res.body() = std::move(boost::json::serialize(msg));
            return std::move(res);
        } 

        constexpr operator Http::response<Body> const&() const&
        {
            return response_;
        }
        constexpr operator Http::response<Body>&() &
        {
            return response_;
        }
        constexpr operator Http::response<Body>&&() &&
        {
            return std::move(response_);
        }
        constexpr operator Http::response<Body>&&() const&&
        {
            return std::move(response_);
        }

        constexpr operator Http::message_generator()
        {
            return std::move(response_);
        }
    };

    using HttpResponse = HttpResponseWrapper<Http::string_body>;
    using HttpResponseFile = HttpResponseWrapper<Http::file_body>;
    using HttpResponseEmpty = HttpResponseWrapper<Http::empty_body>;
}