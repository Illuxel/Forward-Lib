#include "fl/net/http/HttpResponder.hpp"
using namespace Forward;
using namespace Forward::Web;

#include <gtest/gtest.h>

TEST(HttpResponder, RegistrationTest) {

    HttpResponder responder;
    HttpResponder::RouteData route;

    route.Target = "/test";
    route.Method = Http::verb::get;
    route.Handler = [](HttpRequest const& req, HttpResponse&& res)
    {
        return res;
    };

    responder.AddRouteHandler(route);
    
    responder.SetBadHandler(
    [](HttpRequest const& req, Http::status status) 
    {
        HttpResponse res(status, req.base().version());
        res.base().set(Http::field::server, BOOST_BEAST_VERSION_STRING);
        res.base().set(Http::field::content_type, MimeType::FromString("html").GetMimeName());
        res.base().keep_alive(req.base().keep_alive());
        res.base().body() = "Can not find page or resource";
        res.base().prepare_payload();

        return res;
    });

    HttpRequest req(Http::verb::get, "/test", 11);

    responder.HandleRequest(std::move(req));
}