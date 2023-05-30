#pragma once

#include <string>
#include <unordered_map>

namespace fl::utils {

    class MimeType 
    {
        enum class Type;
        enum SubType;

        static const std::unordered_map<std::string_view, MimeType> mime_types_;

        SubType mime_type_; 
        //SubType mime_sub_;

        std::string mime_format_, mime_ext_;

    public:
        enum class Type 
        {

        };

        enum SubType 
        {
            Unknown,

            TextPlain,  
            TextHtml,   // .html, .htm
            TextCss,
            TextCsv,    
            TextJS,

            AppJS, 
            AppJson,
            AppXml,
            AppPdf,
            AppZip,
            AppBinary,
            AppDoc,
            AppDocx,
            AppPpt,
            AppPptx,
            AppXls,
            AppXlsx,

            ImgPng,
            ImgJpeg,    // .jpg, .jpeg, .jpe, .jif, .jfif, .jfi
            ImgGif,
            ImgBmp,
            ImgIco,
            ImgSvg,

            AudioMpeg,  // .mp3, .m4a, .m4b, .m4p, .mpga
            VideoMp4   // .mp4, .m4v
        };

        MimeType();
        MimeType(std::string_view ext);
        MimeType(SubType type, std::string_view format, std::string_view ext = "");

        /**
         *  @return 
         */
        std::string_view GetName(bool remove_dot = true) const;
        /**
         *  @return 
         */
        SubType GetMimeType() const;
        /**
         *  @return 
         */
        std::string_view GetMimeFormat() const;
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
        /**
         *  Will try to remove existing extension from file 
         * 
         *  @param file file name with extension
         *  @return file without extension
         */
        static std::string_view RemoveExtension(std::string_view file, bool remove_dot = true);
        
    private:
        /**
         *  Removes all except extension 
         */
        static std::string_view ExtensionOnly(std::string_view str);
    };
}