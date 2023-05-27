#pragma once


namespace fl::types {

	class Message
	{
	public:
		enum Type {};

		//QByteArray* GetCryptedContent() const;
		//QSslCertificate* GetPublicKey() const;

		//Type GetMessageType() const;
		//QDateTime* GetSendTime() const;

	private:
		// message and key
		//QByteArray* m_EncMsg;
		//QSslCertificate* m_Key;

		// meta data
		//Type m_MsgType;
		//QDateTime* m_SendTime;
	};
}
