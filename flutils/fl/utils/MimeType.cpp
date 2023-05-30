#include "MimeType.hpp"

namespace fl::utils {

    const std::unordered_map<std::string_view, MimeType> MimeType::mime_types_ = 
    {
        { "txt", { MimeType::TextPlain, "text/plain" }},
        { "html",{ MimeType::TextHtml, "text/html" }},
        { "htm", { MimeType::TextHtml, "text/html" }},
        { "css", { MimeType::TextCss, "text/css" }},
        { "js",  { MimeType::TextJavaScript, "text/javascript" }},
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
        , mime_ext_{""} 
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
        , mime_ext_{ext}
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
    std::string_view MimeType::GetExtName(bool remove_dot) const 
    {
        if (!remove_dot)
            return mime_ext_;

        std::string_view without = mime_ext_;
        return without.substr(1, without.size());
    }

    bool MimeType::IsValid() const 
    {
        return mime_type_ != MimeType::Unknown
            || !mime_format_.empty();
    }

    bool MimeType::HasExtension(std::string_view ext)
    {
        auto const& parsed_ext = ExtensionOnly(ext);
        auto const& it = mime_types_.find(parsed_ext);

        return it != mime_types_.end() && !ext.empty();
    }

    MimeType MimeType::FromString(std::string_view ext)
    {
        if (!HasExtension(ext))
            return MimeType();

        auto const sub_ext = ExtensionOnly(ext);
        auto type = mime_types_.at(sub_ext);

        type.mime_ext_.reserve(sub_ext.size() + 1);
        type.mime_ext_.append(".");
        type.mime_ext_.append(sub_ext);

        return type; 
    }

    std::string_view MimeType::RemoveExtension(std::string_view file, bool remove_dot)
    {
        auto ext = MimeType::FromString(file);
        if (!ext.IsValid())
            return file;
    
        auto view = file.substr(0, file.size() - ext.GetExtName(!remove_dot).size());
        return view;
    }
    std::string_view MimeType::ExtensionOnly(std::string_view str)
    {
        auto const dot_pos = str.find('.');    
        auto const is_dot = dot_pos != std::string::npos;

        return str.substr(is_dot ? dot_pos + 1 : 0, str.size());
    }
}