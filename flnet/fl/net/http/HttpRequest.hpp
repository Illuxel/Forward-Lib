#pragma once

#include "fl/net/http/HttpUrl.hpp"

namespace fl {

    template<class Body>
    class HttpRequestWrapper
    {
        http::request<Body> request_data_;

    public:
        HttpRequestWrapper(http::verb method, std::string_view target, int version)
            : request_data_{method, target, version} {}

        HttpRequestWrapper(http::request<Body>&& request) 
            : request_data_(std::move(request)) {}

        template<typename ...Args>
        HttpRequestWrapper(std::piecewise_construct_t t, std::tuple<Args...> body_args) 
            : request_data_{t, std::forward<Args>(body_args)...} {}

        int Version() const {
            return request_data_.version();
        }
        int Version(int version) const {
            return request_data_.version(version);
        }

        bool Alive() const {
            return request_data_.keep_alive();
        }
        void Alive(bool keep_alive) {
            request_data_.keep_alive(keep_alive);
        }

        http::verb Method() const {
            return request_data_.method();
        }
        void Method(http::verb method) {
            request_data_.method(method);
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