#include "fl/net/AsyncListener.hpp"
#include "fl/utils/Log.hpp"


namespace Forward {

    AsyncListener::AsyncListener(net::io_context& ioc,
        Endpoint const& endpoint,
        AsyncAcceptFunc const& call_back)
        : io_context_(ioc)
        , acceptor_ (io_context_)
        , call_back_(call_back) 
        , endpoint_(endpoint) {}

    bool AsyncListener::Listen()
    {
        beast::error_code ec;

        if (Listen(ec))
            return true;

        FL_LOG("Listener", ec.message());

        return false;
    }
    bool AsyncListener::Listen(beast::error_code ec)
    {
        // Open the acceptor
        acceptor_.open(endpoint_.Protocol(), ec);

        if(ec)
            return false;

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);

        if(ec)
            return false;

        // Bind to the server address
        acceptor_.bind(endpoint_, ec);
        if(ec)
            return false;

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections, ec);

        if(ec)
            return false;

        Accept();

        return true;
    }

    void AsyncListener::Accept()
    {
        acceptor_.async_accept(
            net::make_strand(io_context_),
            beast::bind_front_handler(
                call_back_));
    }
        
} // namespace Forward