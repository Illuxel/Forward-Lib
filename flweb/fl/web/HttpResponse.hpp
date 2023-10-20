#pragma once

#include "fl/web/Core.hpp"

namespace Forward::Web {

    template<class Body = Core::StringBody, class Fields = Core::Beast::fields>
    class HttpResponse
    {
    private:
        Core::HttpBaseResponse<Body, Fields> response_;

    public:
        HttpResponse() 
            : response_{} {}

        HttpResponse(HttpStatus status, int version)
            : response_(status, version) {}

        constexpr HttpResponse(Core::HttpBaseResponse<Body, Fields>&& response)
            : response_(std::move(response)) {}
        constexpr HttpResponse(Core::HttpBaseResponse<Body, Fields> const& response)
            : response_(response) {}

        constexpr auto& Base() &
        {
            return response_;
        }
        constexpr auto const& Base() const&
        {
            return response_;
        }
        constexpr auto&& Base() &&
        {
            return std::move(response_);
        }
        constexpr auto const&& Base() const&&
        {
            return std::move(response_);
        }

        void Clear() 
        {
            response_ = {};
        }

        static HttpResponse Message(
            HttpResponse&& res,
            std::string const& msg) 
        {
            res.set(HttpField::content_type, MimeType::FromString("txt").GetMimeName());
            res.body() = std::move(msg);
            return std::move(res);
        } 
        static HttpResponse Message(
            HttpResponse&& res,
            boost::json::value const& msg) 
        {
            res.set(HttpField::content_type, MimeType::FromString("json").GetMimeName());
            res.body() = std::move(boost::json::serialize(msg));
            return std::move(res);
        } 

        constexpr operator Core::Beast::message<false, Body, Fields>& ()&
        {
            return response_;
        }
        constexpr operator Core::Beast::message<false, Body, Fields> const& () const&
        {
            return response_;
        }
        constexpr operator Core::Beast::message<false, Body, Fields> && ()&&
        {
            return std::move(response_);
        }
        constexpr operator Core::Beast::message<false, Body, Fields> const&&() const&&
        {
            return std::move(response_);
        }

        constexpr operator Core::MessageGenerator()
        {
            return std::move(response_);
        }
    };

    using HttpResponseEmpty = HttpResponse<Core::EmptyBody>;
    using HttpResponseString = HttpResponse<Core::StringBody>;
    using HttpResponseFile = HttpResponse<Core::FileBody>;
}