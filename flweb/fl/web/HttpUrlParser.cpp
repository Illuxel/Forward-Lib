#include "fl/web/HttpUrlParser.hpp"

namespace Forward::Web {
    
    std::string HttpUrlParser::EncodeUtf8(std::string_view input)
    {
        std::ostringstream encoded;

        encoded << std::hex << std::uppercase << std::setfill('0');

        for (unsigned char c : input)
        {
            if (c > 127)
            {
                encoded << '%' << std::setw(2) << static_cast<unsigned int>(c);
            }
            else if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                encoded << c;
            }
            else
            {
                encoded << '%' << std::setw(2) << static_cast<unsigned int>(c);
            }
        }

        return encoded.str();
    }
    std::string HttpUrlParser::DecodeUtf8(std::string_view input)
    {
        std::ostringstream decoded;

        for (uint64_t i = 0; i < input.size(); ++i)
        {
            if (input[i] == '%')
            {
                std::string_view hex = input.substr(i + 1, 2);
                unsigned char decode_char = std::stoi(hex.data(), nullptr, 16);

                decoded << decode_char;
                i += 2; // Skip the two hexadecimal characters
            }
            else if (input[i] == '+')
            {
                decoded << ' '; // Replace '+' with space
            }
            else
            {
                decoded << input[i];
            }
        }

        return decoded.str();
    }
} // namespace Forward::Web
