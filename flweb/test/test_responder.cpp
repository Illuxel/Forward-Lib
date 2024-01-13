#include "fl/web/HttpResponder.hpp"
using namespace Forward;
using namespace Forward::Web;

#include <gtest/gtest.h>

TEST(HttpResponder, RegistrationTest) {

    HttpResponder responder;
    HttpResponder::RouteData route;

    route.Target = "/test";
    route.Method = HttpMethod::get;
    route.Handler = 
    [](HttpRequestString const& req, HttpResponseString&& res)
    {
        return res;
    };

    responder.AddRouteHandler(route);
    
    responder.SetBadHandler(
    [](HttpRequestString const& req, HttpStatus status) 
    {
        HttpResponse res(status, req.Base().version());
        res.Base().set(HttpField::server, BOOST_BEAST_VERSION_STRING);
        res.Base().set(HttpField::content_type, MimeType::FromString("html").GetMimeName());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().body() = "Can not find page or resource";
        res.Base().prepare_payload();

        return res;
    });

    HttpRequest req(HttpMethod::get, "/test", 11);

    responder.HandleRequest(std::move(req));
}