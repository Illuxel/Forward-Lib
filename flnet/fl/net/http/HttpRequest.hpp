#pragma once

#include "fl/net/Core.hpp"

#include "fl/net/http/HttpUrl.hpp"

namespace fl {

    // template<typename Type>
    // class HttpRequestWrapper
    // {
    //     http::request<Type> request_data_;

    // public:
    //     HttpRequestWrapper(http::request<Type>&& request) 
    //         : request_data_(std::move(request)) {}

    //     template<typename ...BodyArgs>
    //     HttpRequestWrapper(std::piecewise_construct_t t, std::tuple<BodyArgs...> body_args) 
    //         : request_data_{t, std::forward<BodyArgs>(body_args)...} {}

    //     http::verb GetMethod() const{
    //         return request_data_.method();
    //     }
    //     HttpUrl GetUrl() const {
    //         return HttpUrl(request_data_.target());
    //     }

    //     int GetVersion() const {
    //         return request_data_.version();
    //     }

    //     bool Alive() const {
    //         return request_data_.keep_alive();
    //     }
    //     bool Alive(bool alive) const {
    //         return request_data_.keep_alive(alive);
    //     }
    // };

    using HttpRequest = http::request<http::string_body>;
    using HttpRequestEmpty = http::request<http::empty_body>;
}