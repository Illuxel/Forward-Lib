#include "fl/net/http/HttpResponder.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(HttpResponder, RegistrationTest) {

    HttpResponder responder;
    HttpResponder::HandlerData data;

    data.Target = "/test";
    data.Method = http::verb::get;
    data.Callback = [](HttpRequest const& req, HttpResponse&& res)
    {
        return res;
    };

    responder.AddRouteHandler(data);
    
    responder.SetBadHandler(
    [](HttpRequest const& req, http::status status) 
    {
        HttpResponse res(status, req.base().version());
        res.base().set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.base().set(http::field::content_type, MimeType::FromString("html").GetMimeName());
        res.base().keep_alive(req.base().keep_alive());
        res.base().body() = "Can not find page or resource";
        res.base().prepare_payload();

        return res;
    });

    HttpRequest req(http::verb::get, "/test", 11);

    responder.HandleRequest(std::move(req));
}