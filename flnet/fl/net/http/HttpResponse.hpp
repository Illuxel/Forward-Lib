#pragma once

#include "fl/net/Core.hpp"

namespace fl {

    // template<typename Type>
    // class HttpResponseWrapper
    // {
    //     http::response<Type> response_data_;

    // public:
    //     HttpResponseWrapper(http::status status, int version)
    //         : response_data_{status, version} {}

    //     HttpResponseWrapper(http::response<Type>&& response)
    //         : response_data_{std::move(response)} {}
    //     HttpResponseWrapper(http::response<Type> const& response)
    //         : response_data_{response} {}

    //     template<typename ...BodyArgs>
    //     HttpResponseWrapper(std::piecewise_construct_t t, std::tuple<BodyArgs...> body_args) 
    //         : response_data_{t, std::forward<BodyArgs>(body_args)...} {}

    //     void SetHeader(http::field field, std::string_view data) {
    //         request_data_.set(field, data);
    //     }

    //     std::string& Body() & {
    //         return request_data_.body();
    //     }
    //     std::string&& Body() && {
    //         return std::move(request_data_.body());
    //     }
    // };

    using HttpResponse = http::response<http::string_body>;
    using HttpResponseEmpty = http::response<http::empty_body>;
}