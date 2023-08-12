#pragma once

#include "fl/net/http/HttpUrl.hpp"

namespace Forward::Web {

    template<class Body>
    class HttpRequestWrapper
    {
    private:
        Http::request<Body> request_;

    public:
        HttpRequestWrapper()
            : request_{} {}

        HttpRequestWrapper(Http::status status, int version)
            : request_(status, version) {}
        HttpRequestWrapper(Http::verb method, std::string_view target, int version)
            : request_(method, target, version) {}

        HttpRequestWrapper(Http::request<Body>&& request)
            : request_(std::move(request)){}
        HttpRequestWrapper(Http::request<Body> const& request)
            : request_(request) {}

        constexpr auto const& base() const &
        {
            return request_;
        }
        constexpr auto& base() &
        {
            return request_;
        }
        constexpr auto&& base() &&
        {
            return std::move(request_);
        }
        constexpr auto&& base() const&&
        {
            return std::move(request_);
        }

        HttpUrl Url() const 
        {
            return HttpUrl(request_.target());
        }
        HttpQuery Query() const 
        {
            HttpQuery query(Url().Query());

            if (query.IsValid())
                return query;

            query.SetQuery(request_.body());
            return query;
        }
        
        void Clear() 
        {
            request_ = {};
        }

        constexpr operator Http::request<Body>&() &
        {
            return &request_;
        }
        constexpr operator Http::request<Body>&&() &&
        {
            return std::move(request_);
        }
        constexpr operator Http::request<Body> && () const&&
        {
            return std::move(request_);
        }
        constexpr operator Http::request<Body> const&() const&
        {
            return request_;
        }

        constexpr operator Http::message_generator()
        {
            return std::move(request_);
        }
    };

    using HttpRequest = HttpRequestWrapper<Http::string_body>;
    using HttpRequestFile = HttpRequestWrapper<Http::file_body>;
    using HttpRequestEmpty = HttpRequestWrapper<Http::empty_body>;
}