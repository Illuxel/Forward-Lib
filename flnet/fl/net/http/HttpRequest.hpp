#pragma once

#include "fl/net/http/HttpUrl.hpp"

namespace fl {

    template<typename Type>
    class HttpRequestWrapper
    {
        http::request<Type> request_data_;

    public:
        HttpRequestWrapper(http::request<Type>&& request) 
            : request_data_(std::move(request)) {}

        template<typename ...Args>
        HttpRequestWrapper(std::piecewise_construct_t t, std::tuple<Args...> body_args) 
            : request_data_{t, std::forward<Args>(body_args)...} {}

        int Version() const {
            return request_data_.version();
        }
        bool Alive() const {
            return request_data_.keep_alive();
        }
        http::verb Method() const {
            return request_data_.method();
        }

        HttpUrl Url() const {
            return HttpUrl(request_data_.target());
        }
        HttpQuery Query() const {
            HttpQuery query(Url().Query());

            if (query.IsValid())
                return query;

            query.SetQuery(request_data_.body());
            return query;
        }
        
        auto Body() const {
            return request_data_.body();
        }
    };

    using HttpRequest = HttpRequestWrapper<http::string_body>;
    using HttpRequestEmpty = HttpRequestWrapper<http::empty_body>;
}