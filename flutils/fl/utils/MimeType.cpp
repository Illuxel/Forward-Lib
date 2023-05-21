#include "MimeType.hpp"

namespace fl::utils {

    const std::unordered_map<std::string_view, MimeType> MimeType::mime_types_ = 
    {
        { "txt", { MimeType::TextPlain, "text/plain" }},
        { "html",{ MimeType::TextHtml, "text/html" }},
        { "htm", { MimeType::TextHtml, "text/html" }},
        { "json",{ MimeType::AppJson, "application/json" }},
        { "xml", { MimeType::AppXml, "application/xml" }},
        { "pdf", { MimeType::AppPdf, "application/pdf" }},
        { "zip", { MimeType::AppZip, "application/zip" }},
        { "7z",  { MimeType::AppZip, "application/x-7z-compressed" }},
        { "bin", { MimeType::AppBinary, "application/octet-stream" }},
        { "jpg", { MimeType::ImgJpeg, "image/jpeg" }},
        { "jpeg",{ MimeType::ImgJpeg, "image/jpeg" }},
        { "jpe", { MimeType::ImgJpeg, "image/jpeg" }},
        { "jif", { MimeType::ImgJpeg, "image/jpeg" }},
        { "jfif",{ MimeType::ImgJpeg, "image/jpeg" }},
        { "jfi", { MimeType::ImgJpeg, "image/jpeg" }},
        { "png", { MimeType::ImgPng, "image/png" }},
        { "mp3", { MimeType::AudioMpeg, "audio/mpeg" }},
        { "m4a", { MimeType::AudioMpeg, "audio/mpeg" }},
        { "m4b", { MimeType::AudioMpeg, "audio/mpeg" }},
        { "m4p", { MimeType::AudioMpeg, "audio/mpeg" }},
        { "mpga",{ MimeType::AudioMpeg, "audio/mpeg" }},
        { "mp4", { MimeType::VideoMp4, "video/mp4" }},
        { "m4v", { MimeType::VideoMp4, "video/mp4" }}
    };

    MimeType::MimeType()
        : mime_type_{Type::Unknown}
        , mime_format_{""}
        , extension_{""} 
    {

    }
    MimeType::MimeType(std::string_view ext)
    {
        *this = MimeType::FromString(ext);
    }
    MimeType::MimeType(Type mime_type, 
        std::string_view mime_format, 
        std::string_view ext)
        : mime_type_{mime_type}
        , mime_format_{mime_format}
        , extension_{ext}
    {
        
    }

    MimeType::Type MimeType::GetMimeType() const
    {
        return mime_type_;
    }
    std::string_view MimeType::GetMimeFormat() const 
    {
        return mime_format_;
    }
    std::string_view MimeType::GetExtName() const 
    {
        return extension_;
    }

    bool MimeType::IsValid() const 
    {
        return mime_type_ != MimeType::Unknown
            || !mime_format_.empty();
    }
    bool MimeType::IsExtension(std::string_view ext)
    {
        auto const parsed_ext = SubStrExtFromString(ext);
        auto const it = mime_types_.find(parsed_ext);

        return it != mime_types_.end();
    }

    MimeType MimeType::FromString(std::string_view ext)
    {
        if (!IsExtension(ext))
            return MimeType();

        auto const sub_ext = SubStrExtFromString(ext);
        auto type = mime_types_.at(sub_ext);

        type.extension_ = sub_ext;

        return type; 
    }

    std::string_view MimeType::SubStrExtFromString(std::string_view str)
    {
        auto const dot_pos = str.find('.');    
        auto const is_dot = dot_pos != std::string::npos;

        return str.substr(is_dot ? dot_pos + 1 : 0, str.size());
    }
}