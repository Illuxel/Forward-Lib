#pragma once

#include <string>
#include <unordered_map>

namespace fl::utils {

    class MimeType 
    {
        enum Type;

        Type mime_type_;
        std::string mime_format_, mime_ext_;

        static const std::unordered_map<std::string_view, MimeType> mime_types_;

    public:
        enum Type {
            Unknown,

            TextPlain,  
            TextHtml,   // .html, .htm
            TextCss,    

            AppJavaScript, 
            AppJson,
            AppXml,
            AppPdf,

            ImgPng,
            ImgJpeg,    // .jpg, .jpeg, .jpe, .jif, .jfif, .jfi
            ImgGif,

            AudioMpeg,  // .mp3, .m4a, .m4b, .m4p, .mpga
            VideoMp4,   // .mp4, .m4v

            AppZip,
            AppBinary
        };

        MimeType();
        MimeType(std::string_view ext);
        MimeType(Type type, std::string_view format, std::string_view ext = "");

        /**
         *  @return 
         */
        Type GetMimeType() const;
        /**
         *  @return 
         */
        std::string_view GetMimeFormat() const;
        /**
         *  @return 
         */
        std::string_view GetExtName(bool dot = false) const;
        /**
         *  @return true if extension is valid
         */
        bool IsValid() const;
        /**
         *  @param ext extension names (txt, .txt, filename.txt)
         *  @return true if extension is valid
         */
        static bool HasExtension(std::string_view ext);
        /**
         *  @param ext extension names (txt, .txt, filename.txt)
         *  @return MimeType object. Can be valid or not depending on provided string
         */
        static MimeType FromString(std::string_view ext);

    private:
        /**
         *  Removes dot from extension
         */
        static std::string_view SubStrExtFromString(std::string_view str);
    };
}