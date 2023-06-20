#include "fl/net/http/HttpResponder.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(HttpResponder, RegistrationTest) {

    HttpResponder responder;
    HttpResponder::HandlerData data;

    data.Target = "/test";
    data.Method = http::verb::get;
    data.Callback = [](HttpRequest const& req, HttpResponse&& res)
    {
        return std::move(res);
    };

    responder.AddRouteHandler(data);
    
    responder.SetBadHandler(
    [](HttpRequest const& req, http::status status) 
    {
        HttpResponse res{ status, req.Version() };
        res.SetHeader(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.SetHeader(http::field::content_type, MimeType::FromString("html").GetMimeName());
        res.SetAlive(req.Alive());
        res.Body() = "Can not find page or resource";
        res.Prepare();

        return std::move(res);
    });

    HttpRequest req{http::verb::get, "test", 1};

    responder.HandleRequest(std::move(req));
}