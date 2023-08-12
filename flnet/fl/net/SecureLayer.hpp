#pragma once

#include "fl/net/Core.hpp"

namespace Forward::Net {

	class SecureLayer 
	{
	private:
		Core::Ssl::context secure_context_;

	public:
		SecureLayer(Core::Ssl::context::method method);

		virtual ~SecureLayer();

		void SetupFileSslCert(std::string_view filename,
			Core::Ssl::context::file_format format = Core::Ssl::context::pem);
		void SetupFileSslCertKey(std::string_view filename, std::string_view pass = "",
			Core::Ssl::context::file_format format = Core::Ssl::context::pem);
	};
}
