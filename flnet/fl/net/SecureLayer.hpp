#pragma once

#include "fl/net/Core.hpp"

namespace Forward {

	class SecureLayer 
	{
	private:
		ssl::context secure_context_;

	public:
		SecureLayer(ssl::context::method method);

		void SetupFileSslCert(std::string_view filename,
			net::ssl::context::file_format format = net::ssl::context::pem);
		void SetupFileSslCertKey(std::string_view filename, std::string_view pass = "",
			net::ssl::context::file_format format = net::ssl::context::pem);
	};
}
