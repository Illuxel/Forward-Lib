#include "aloAPI/datatypes/Message.hpp"

namespace alo {

    QByteArray* Message::GetCryptedContent() const
    {
        return nullptr;
    }

    QSslCertificate* alo::Message::GetPublicKey() const
    {
        return nullptr;
    }

    Message::Type Message::GetMessageType() const
    {
        return Type();
    }

    QDateTime* Message::GetSendTime() const
    {
        return nullptr;
    }
}