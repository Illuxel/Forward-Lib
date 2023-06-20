#pragma once

#include "fl/net/http/Core.hpp"

namespace fl {

    template<typename Body>
    class HttpResponseWrapper
    {
        http::response<Body> response_data_;

    public:
        HttpResponseWrapper(http::status status, int version)
            : response_data_{status, version} {}

        HttpResponseWrapper(http::response<Body>&& response)
            : response_data_{std::move(response)} {}
        HttpResponseWrapper(http::response<Body> const& response)
            : response_data_{response} {}

        template<typename ...BodyArgs>
        HttpResponseWrapper(std::piecewise_construct_t t, std::tuple<BodyArgs...> body_args) 
            : response_data_{t, std::forward<BodyArgs>(body_args)...} {}

        void SetAlive(bool keep_alive) 
        {
            response_data_.keep_alive(keep_alive);
        }
        void SetHeader(http::field field, std::string_view data) 
        {
            response_data_.set(field, data);
        }
        void SetSize(size_t size) 
        {
            response_data_.content_length(size);
        }

        auto& Body() & 
        {
            return response_data_.body();
        }
        auto&& Body() && 
        {
            return std::move(response_data_.body());
        }

        void Prepare() 
        {
            response_data_.prepare_payload();
        }

        template<typename Body, typename T>
        static HttpResponseWrapper<Body> Message(HttpResponseWrapper<Body> res, T const& msg) 
        {
            auto& ref = MessageImpl(std::move(res), msg);
            ref.Prepare();
            return std::move(ref);
        }

        operator http::message_generator() 
        {
            return std::move(response_data_);
        }

    private: 
        template<typename Body>
        static HttpResponseWrapper<Body> MessageImpl(HttpResponseWrapper<Body>&& res, std::string const& msg) 
        {
            res.SetHeader(http::field::content_type, "text/plain");
            res.Body() = std::move(msg);
            return std::move(res);
        } 
        template<typename Body>
        static HttpResponseWrapper<Body> MessageImpl(HttpResponseWrapper<Body>&& res, boost::json::value const& msg) 
        {
            res.SetHeader(http::field::content_type, "application/json");
            res.Body() = std::move(boost::json::serialize(msg));
            return std::move(res);
        } 
    };

    using HttpResponse = HttpResponseWrapper<http::string_body>;
    using HttpResponseFile = HttpResponseWrapper<http::file_body>;
    using HttpResponseEmpty = HttpResponseWrapper<http::empty_body>;
}