#include "fl/net/AsyncListener.hpp"

namespace fl {

    AsyncListener::AsyncListener(net::io_context& ioc,
        Endpoint const& endpoint,
        AsyncAcceptFunc const& call_back)
        : io_context_(ioc)
        , acceptor_ (net::make_strand(io_context_), endpoint)
        , call_back_(call_back) {}

    bool AsyncListener::Listen()
    {
        beast::error_code ec;
        return Listen(ec);
    }
    bool AsyncListener::Listen(beast::error_code ec)
    {
        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);

        if(ec)
            return false;

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections, ec);

        if(ec)
            return false;

        net::dispatch(
            acceptor_.get_executor(),
            beast::bind_front_handler(
                &AsyncListener::Accept,
                this->shared_from_this()));

        return true;
    }

    void AsyncListener::Accept()
    {
        acceptor_.async_accept(
            net::make_strand(io_context_),
            beast::bind_front_handler(call_back_));
    }
        
} // namespace fl