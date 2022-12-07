#pragma once

#include "aloAPI/alo-common.hpp"

class QSslCertificate;
class QDateTime;

namespace alo {

	class ALO_API Message
	{
	public:
		enum Type {};

		QByteArray* GetCryptedContent() const;
		QSslCertificate* GetPublicKey() const;

		Type GetMessageType() const;
		QDateTime* GetSendTime() const;

	private:
		// message and key
		QByteArray* m_EncMsg;
		QSslCertificate* m_Key;

		// meta data
		Type m_MsgType;
		QDateTime* m_SendTime;
	};
}
