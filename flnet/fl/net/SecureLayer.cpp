#include "fl/net/SecureLayer.hpp"

#include "fl/utils/Log.hpp"

namespace Forward {

	SecureLayer::SecureLayer(ssl::context::method method)
		: secure_context_(method) {}

    void SecureLayer::SetupFileSslCert(std::string_view filename,
        net::ssl::context::file_format format)
    {
        sys::error_code ec;

        secure_context_.use_certificate_chain_file(filename.data(), ec);

        if (ec)
        {
            FL_LOG("SecureLayer", ec);
        }
    }
    void SecureLayer::SetupFileSslCertKey(std::string_view filename, std::string_view pass,
        net::ssl::context::file_format format)
    {
        sys::error_code ec;

        if (!pass.empty())
        {
            secure_context_.set_password_callback(
            [&](uint64_t, net::ssl::context::password_purpose) 
            {
                return pass.data();
            }, ec);

            if (ec)
            {
                FL_LOG("SecureLayer", ec);
                return;
            }
        }

        secure_context_.use_private_key_file(filename.data(), format, ec);

        if (ec)
        {
            FL_LOG("SecureLayer", ec);
        }
    }
}
