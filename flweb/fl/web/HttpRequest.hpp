#pragma once

#include "fl/web/HttpUrl.hpp"

namespace Forward::Web {

    template<class Body = Core::StringBody, class Fields = Core::Beast::fields>
    class HttpRequest
    {
    private:
        Core::HttpBaseRequest<Body, Fields> request_;

    public:
        HttpRequest()
            : request_{} {}

        HttpRequest(HttpStatus status, int version)
            : request_(status, version) {}
        HttpRequest(HttpMethod method, std::string_view target, int version)
            : request_(method, target, version) {}

        HttpRequest(Core::HttpBaseRequest<Body>&& request)
            : request_(request) {}
        HttpRequest(Core::HttpBaseRequest<Body> const& request)
            : request_(request) {}

        constexpr auto& Base() &
        {
            return request_;
        }
        constexpr auto&& Base() &&
        {
            return std::move(request_);
        }
        constexpr auto&& Base() const&&
        {
            return std::move(request_);
        }
        constexpr auto const& Base() const &
        {
            return request_;
        }

        HttpUrl Url() const 
        {
            return HttpUrl(request_.target());
        }
      
        void Clear() 
        {
            request_ = {};
        }

        constexpr operator Core::Beast::message<true, Body, Fields> &() &
        {
            return request_;
        }
        constexpr operator Core::Beast::message<true, Body, Fields> const&() const&
        {
            return request_;
        }
        constexpr operator Core::Beast::message<true, Body, Fields> &&() &&
        {
            return request_;
        }
        constexpr operator Core::Beast::message<true, Body, Fields> const&&() const&&
        {
            return request_;
        }

        constexpr operator Core::MessageGenerator()
        {
            return std::move(request_);
        }
    };

    using HttpRequestEmpty = HttpRequest<Core::EmptyBody>;
    using HttpRequestString = HttpRequest<Core::StringBody>;
    using HttpRequestFile = HttpRequest<Core::FileBody>;
} // namespace Forward::Web