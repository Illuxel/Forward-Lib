#include "fl/utils/MimeType.hpp"
#include "fl/utils/Exception.hpp"

namespace Forward {

    const std::map<std::string_view, MimeType> MimeType::mime_types_ 
    {
        { "txt",  { MimeType::Text, MimeType::SubType::Plain,  "plain" }},
        { "htm",  { MimeType::Text, MimeType::SubType::Html,   "html" }},
        { "html", { MimeType::Text, MimeType::SubType::Html,   "html" }},
        { "csv",  { MimeType::Text, MimeType::SubType::Csv,    "csv" }},
        { "css",  { MimeType::Text, MimeType::SubType::Css,    "css" }},
        { "php",  { MimeType::Text, MimeType::SubType::Php,    "php" }},
        { "jpg",  { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "jpeg", { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "jpe",  { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "jif",  { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "jfif", { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "jfi",  { MimeType::Img, MimeType::SubType::Jpeg,    "jpeg" }},
        { "png",  { MimeType::Img, MimeType::SubType::Png,     "png" }},
        { "gif",  { MimeType::Img, MimeType::SubType::Gif,     "gif" }},
        { "bmp",  { MimeType::Img, MimeType::SubType::Bmp,     "bmp" }},
        { "ico",  { MimeType::Img, MimeType::SubType::Ico,     "x-icon" }},
        { "svg",  { MimeType::Img, MimeType::SubType::Svg,     "svg+xml" }},
        { "svgz", { MimeType::Img, MimeType::SubType::Svg,    "svg+xml" }},
        { "mp3",  { MimeType::Audio, MimeType::SubType::Mpeg,  "mpeg" }},
        { "m4a",  { MimeType::Audio, MimeType::SubType::Mpeg,  "mpeg" }},
        { "m4b",  { MimeType::Audio, MimeType::SubType::Mpeg,  "mpeg" }},
        { "m4p",  { MimeType::Audio, MimeType::SubType::Mpeg,  "mpeg" }},
        { "mpga", { MimeType::Audio, MimeType::SubType::Mpeg,  "mpeg" }},
        { "mp4",  { MimeType::Video, MimeType::SubType::Mp4,   "mp4" }},
        { "m4v",  { MimeType::Video, MimeType::SubType::Mp4,   "mp4" }},
        { "js",   { MimeType::App, MimeType::SubType::JavaScript,"javascript" }},
        { "json", { MimeType::App, MimeType::SubType::Json,    "json" }},
        { "xml",  { MimeType::App, MimeType::SubType::Xml,     "xml" }},
        { "pdf",  { MimeType::App, MimeType::SubType::Pdf,     "pdf" }},
        { "zip",  { MimeType::App, MimeType::SubType::Zip,     "zip" }},
        { "7z",   { MimeType::App, MimeType::SubType::Zip,     "x-7z-compressed" }},
        { "exe",  { MimeType::App, MimeType::SubType::Exe,     "x-msdownload" }},
        { "bin",  { MimeType::App, MimeType::SubType::Binary,  "octet-stream" }},
        { "doc",  { MimeType::App, MimeType::SubType::Doc,     "msword" }},
        { "xls",  { MimeType::App, MimeType::SubType::Xls,     "vnd.ms-excel" }},
        { "ppt",  { MimeType::App, MimeType::SubType::Ppt,     "vnd.ms-powerpoint" }},
        { "docx", { MimeType::App, MimeType::SubType::Docx,    "vnd.openxmlformats-officedocument.wordprocessingml.document" }},
        { "xlsx", { MimeType::App, MimeType::SubType::Xlsx,    "vnd.openxmlformats-officedocument.spreadsheetml.sheet" }},
        { "pptx", { MimeType::App, MimeType::SubType::Pptx,    "vnd.openxmlformats-officedocument.presentationml.presentation" }}
    };

    static constexpr inline const char* EnumTypeToString(MimeType::Type type)
    {
        switch (type)
        {
        case MimeType::Text: 
            return "text";
        case MimeType::App: 
            return "application";
        case MimeType::Img: 
            return "image";
        case MimeType::Audio: 
            return "audio";
        case MimeType::Video: 
            return "video";
        case MimeType::Msg: 
            return "message";
        case MimeType::Multi: 
            return "multipart";
        case MimeType::Model: 
            return "model";
        case MimeType::Example: 
            return "example";
        }

        throw Exception("Specified unknown mime type");
    }

    static constexpr inline const char* CreateSubTypeString(MimeType::SubType sub_type)
    {
        //throw Exception("Specified unknown mime subtype");
        return nullptr;
    }

    MimeType::MimeType()
        : sub_type_str_("*") {}

    MimeType::MimeType(std::string_view ext)
    {
        *this = MimeType::FromString(ext);
    }

    MimeType::MimeType(Type type, SubType sub_type, std::string_view sub_type_str, std::string_view ext)
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
        return EnumTypeToString(type_) + '/' + sub_type_str_;
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
        std::string_view only_ext = ExtensionOnly(ext);
        auto const& it = mime_types_.find(only_ext);

        return it != mime_types_.end() && !ext.empty();
    }

    MimeType MimeType::FromString(std::string_view ext)
    {
        if (!HasExtension(ext))
            return MimeType();

        std::string_view only_ext = ExtensionOnly(ext);
        MimeType mime_type = mime_types_.at(only_ext);

        mime_type.ext_name_ = only_ext;

        return mime_type; 
    }

    std::string MimeType::RemoveExtension(std::string_view file)
    {
        MimeType mime_type = MimeType::FromString(file);

        if (!mime_type.IsValid())
            return file.data();
    
        return std::string(file.substr(0, file.size() - mime_type.GetExtName(false).size()));
    }
    std::string_view MimeType::ExtensionOnly(std::string_view str)
    {
        uint64_t const dot_pos = str.find('.');    
        uint64_t const is_dot = dot_pos != std::string::npos;

        return str.substr(is_dot ? dot_pos + 1 : 0, str.size());
    }
}