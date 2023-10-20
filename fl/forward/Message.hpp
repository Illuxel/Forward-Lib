#pragma once

namespace Forward::Data {

	class Message
	{
	public:
		enum Type {};

		//QByteArray* GetCryptedContent() const;
		//QSSLCertificate* GetPublicKey() const;

		//Type GetMessageType() const;
		//QDateTime* GetSendTime() const;

	private:
		// message and key
		//QByteArray* m_EncMsg;
		//QSSLCertificate* m_Key;

		// meta data
		//Type m_MsgType;
		//QDateTime* m_SendTime;
	};
}
