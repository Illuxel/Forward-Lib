#include "fl/forward/Client.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(Client, Connection)
{
    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv13_client};

    // This holds the root certificate used for verification
    // load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // Launch the asynchronous operation
    // The session is constructed with a strand to
    // ensure that handlers do not execute concurrently.
    // MakeRef<Client>(
    //     net::make_strand(ioc),
    //     ctx
    //     )->Run("forchat.online", "8080", "/login", 11);

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();
}