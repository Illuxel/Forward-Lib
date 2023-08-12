#include "fl/net/SecureLayer.hpp"

#include "fl/utils/Log.hpp"

namespace Forward::Net {

	SecureLayer::SecureLayer(
        Core::Ssl::context::method method)
		: secure_context_(method) {}

    SecureLayer::~SecureLayer()
    {
    }

    void SecureLayer::SetupFileSslCert(
        std::string_view filename,
        Core::Ssl::context::file_format format)
    {
        Core::Error ec;

        secure_context_.use_certificate_chain_file(filename.data(), ec);

        if (ec)
        {
            FL_LOG("SecureLayer", ec);
        }
    }
    void SecureLayer::SetupFileSslCertKey(
        std::string_view filename, 
        std::string_view pass,
        Core::Ssl::context::file_format format)
    {
        Core::Error ec;

        if (!pass.empty())
        {
            secure_context_.set_password_callback(
            [&pass](uint64_t, Core::Ssl::context::password_purpose) 
            {
                return pass.data();
            }, ec);

            if (ec)
            {
                return FL_LOG("SecureLayer", ec);
            }
        }

        secure_context_.use_private_key_file(filename.data(), format, ec);

        if (ec)
        {
            FL_LOG("SecureLayer", ec);
        }
    }
}
