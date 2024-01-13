#pragma once

#include "fl/forward/Core.hpp"

namespace Forward::API {

	class FORWARD_API Message
	{
	public:
		enum Type {};

		//GetCryptedContent() const;
		//GetPublicKey() const;

		//Type GetMessageType() const;
		//GetSendTime() const;

	private:
		// message and key
		//QByteArray* m_EncMsg;
		//QSSLCertificate* m_Key;

		// meta data
		//Type m_MsgType;
		//QDateTime* m_SendTime;
	};
}
