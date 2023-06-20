#include "MimeType.hpp"

namespace fl {

    const std::map<std::string_view, MimeType> MimeType::mime_types_ = 
    {
        { "txt",  { Type::Text, SubType::Plain,  "plain" }},
        { "htm",  { Type::Text, SubType::Html,   "html" }},
        { "html", { Type::Text, SubType::Html,   "html" }},
        { "csv",  { Type::Text, SubType::Csv,    "csv" }},
        { "css",  { Type::Text, SubType::Css,    "css" }},
        { "php",  { Type::Text, SubType::Php,    "php" }},
        { "jpg",  { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "jpeg", { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "jpe",  { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "jif",  { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "jfif", { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "jfi",  { Type::Img, SubType::Jpeg,    "jpeg" }},
        { "png",  { Type::Img, SubType::Png,     "png" }},
        { "gif",  { Type::Img, SubType::Gif,     "gif" }},
        { "bmp",  { Type::Img, SubType::Bmp,     "bmp" }},
        { "ico",  { Type::Img, SubType::Ico,     "x-icon" }},
        { "svg",  { Type::Img, SubType::Svg,     "svg+xml" }},
        { "svgz",  { Type::Img, SubType::Svg,    "svg+xml" }},
        { "mp3",  { Type::Audio, SubType::Mpeg,  "mpeg" }},
        { "m4a",  { Type::Audio, SubType::Mpeg,  "mpeg" }},
        { "m4b",  { Type::Audio, SubType::Mpeg,  "mpeg" }},
        { "m4p",  { Type::Audio, SubType::Mpeg,  "mpeg" }},
        { "mpga", { Type::Audio, SubType::Mpeg,  "mpeg" }},
        { "mp4",  { Type::Video, SubType::Mp4,   "mp4" }},
        { "m4v",  { Type::Video, SubType::Mp4,   "mp4" }},
        { "js",   { Type::App, SubType::JavaScript,"javascript" }},
        { "json", { Type::App, SubType::Json,    "json" }},
        { "xml",  { Type::App, SubType::Xml,     "xml" }},
        { "pdf",  { Type::App, SubType::Pdf,     "pdf" }},
        { "zip",  { Type::App, SubType::Zip,     "zip" }},
        { "7z",   { Type::App, SubType::Zip,     "x-7z-compressed" }},
        { "exe",  { Type::App, SubType::Exe,     "x-msdownload" }},
        { "bin",  { Type::App, SubType::Binary,  "octet-stream" }},
        { "doc",  { Type::App, SubType::Doc,     "msword" }},
        { "xls",  { Type::App, SubType::Xls,     "vnd.ms-excel" }},
        { "ppt",  { Type::App, SubType::Ppt,     "vnd.ms-powerpoint" }},
        { "docx", { Type::App, SubType::Docx,    "vnd.openxmlformats-officedocument.wordprocessingml.document" }},
        { "xlsx", { Type::App, SubType::Xlsx,    "vnd.openxmlformats-officedocument.spreadsheetml.sheet" }},
        { "pptx", { Type::App, SubType::Pptx,    "vnd.openxmlformats-officedocument.presentationml.presentation" }}
    };

    static inline std::string CreateTypeString(MimeType::Type type)
    {
        switch (type)
        {
        case MimeType::Text: 
            return "text/";
        case MimeType::App: 
            return "application/";
        case MimeType::Img: 
            return "image/";
        case MimeType::Audio: 
            return "audio/";
        case MimeType::Video: 
            return "video/";
        case MimeType::Msg: 
            return "message/";
        case MimeType::Multi: 
            return "multipart/";
        case MimeType::Model: 
            return "model/";
        case MimeType::Example: 
            return "example/";
        }

        return "";
    }

    static inline std::string CreateSubTypeString(MimeType::SubType sub_type)
    {
        return "";
    }

    MimeType::MimeType()
        : type_{MimeType::App}
        , sub_type_{SubType::Unknown}
        , sub_type_str_{"*"}
        , ext_name_{""} {}
    MimeType::MimeType(std::string_view ext)
    {
        *this = MimeType::FromString(ext);
    }
    MimeType::MimeType(Type type, SubType sub_type,  std::string_view sub_type_str, std::string_view ext)
        : type_{type}
        , sub_type_{sub_type}
        , sub_type_str_{sub_type_str}
        , ext_name_{ext} {}

    std::string MimeType::GetExtName(bool remove_dot) const 
    {
        if (!remove_dot)
            return std::string('.' + ext_name_);

        return ext_name_;
    }

    MimeType::Type MimeType::GetType() const
    {
        return type_;
    }
    MimeType::SubType MimeType::GetSubType() const
    {
        return sub_type_;
    }
    std::string MimeType::GetMimeName() const 
    {
        return CreateTypeString(type_) + sub_type_str_;
    }

    bool MimeType::IsUnknown() const 
    {
        return type_ == MimeType::App
             && sub_type_ != MimeType::SubType::Unknown
             && sub_type_str_ == "*";
    }
    bool MimeType::IsValid() const 
    {
        return sub_type_ != MimeType::SubType::Unknown;
    }
    
    bool MimeType::HasExtension(std::string_view ext)
    {
        auto const& only_ext = ExtensionOnly(ext);
        auto const& it = mime_types_.find(only_ext);

        return it != mime_types_.end() && !ext.empty();
    }

    MimeType MimeType::FromString(std::string_view ext)
    {
        if (!HasExtension(ext))
            return MimeType();

        auto only_ext = ExtensionOnly(ext);
        auto mime_type = mime_types_.at(only_ext);

        mime_type.ext_name_ = only_ext;

        return mime_type; 
    }

    std::string MimeType::RemoveExtension(std::string_view file)
    {
        auto const& ext = MimeType::FromString(file);
        if (!ext.IsValid())
            return file.data();
    
        return std::string(file.substr(0, file.size() - ext.GetExtName(false).size()));
    }
    std::string_view MimeType::ExtensionOnly(std::string_view str)
    {
        auto const dot_pos = str.find('.');    
        auto const is_dot = dot_pos != std::string::npos;

        return str.substr(is_dot ? dot_pos + 1 : 0, str.size());
    }
}