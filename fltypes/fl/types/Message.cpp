#include "fl/types/Message.hpp"

namespace fl::types {

    QByteArray* Message::GetCryptedContent() const
    {
        return nullptr;
    }

    QSslCertificate* Message::GetPublicKey() const
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