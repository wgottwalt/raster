#pragma once

#include <iosfwd>
#include <limits>
#include <string>
#include "Base.hxx"

namespace Image
{
    //
    // Targa - the good old image format well suited for textures
    //
    // - supports all mapped, truecolor and mono image types
    // - proper handles 15/16 bit data even in mapped mode
    // - recognizes Targa spec 1.0 and 2.0, but only writes empty 2.0 footers (supporting this would
    //   blow this project out of scope, no seriously, did someone ever use the footers?!?)
    // - supports the fuckup in the spec 1.0 and 2.0, aka scanlined and non-scanlined RLE encoding
    //   (how Targa RLE works was tested in the Simple01 image format, I'm going to use this here)
    // - there are two propreitary pixel encodings I can not support, because I'm not able to find
    //   a spec descriping these
    //
    class Targa : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 0;
        static constexpr int64_t MinHeight = 0;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint16_t>::max();
        static constexpr int64_t MaxHeight = MaxWidth;

        enum class ImageType : uint8_t {
            NoData = 0,
            Mapped = 1,
            Truecolor = 2,
            Mono = 3,
            MappedRLE = 9,
            TruecolorRLE = 10,
            MonoRLE = 11,
            MappedAll = 32,
            MappedAllQuad = 33
        };

        enum class ImageAttribute : uint8_t {
            OriginRight = 16,
            OriginTop = 32
        };

        struct Header {
            uint8_t id;
            uint8_t colormap_type;
            ImageType image_type;
            uint16_t colormap_offset;
            uint16_t colormap_length;
            uint8_t colormap_entry_size;
            uint16_t x_origin;
            uint16_t y_origin;
            uint16_t width;
            uint16_t height;
            uint8_t depth;
            uint8_t image_descriptor;
        } __attribute__((packed));

        struct Footer {
            uint32_t ext_offset;
            uint32_t dev_offset;
            char signature[16];
            char end[2];
        } __attribute__((packed));

        //--- public constructors ---
        Targa() noexcept;
        Targa(const std::string &filename) noexcept(false);
        Targa(const int64_t width, const int64_t height, const RGBA color = RGBA::Black)
            noexcept(false);
        Targa(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        Targa(const Targa &rhs) noexcept(false);
        Targa(Targa &&rhs) noexcept;
        virtual ~Targa() noexcept;

        //--- public operators ---
        Targa &operator=(const Targa &rhs) noexcept(false);
        Targa &operator=(Targa &&rhs) noexcept;
        bool operator==(const Targa &rhs) const noexcept;
        bool operator!=(const Targa &rhs) const noexcept;

        //--- public methods ---
        std::string id() const noexcept;
        void setId(const std::string &str) noexcept(false);
        ImageType imageType() const noexcept;
        void setImageType(const ImageType type) noexcept(false);
        uint8_t depth() const noexcept;
        bool setDepth(const uint8_t val) noexcept;
        bool isVersion2() const noexcept;
        void setVersion2(const bool val) noexcept;
        int64_t xOrigin() const noexcept;
        bool setXOrigin(const int64_t val) noexcept;
        int64_t yOrigin() const noexcept;
        bool setYOrigin(const int64_t val) noexcept;
        bool greyscaleMonochromeMode() const noexcept;
        void setGreyscaleMonochromeMode(const bool val) noexcept;

        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) override final;
        virtual bool save(const std::string &filename) const noexcept(false) override final;
        virtual bool load(const std::string &filename) noexcept(false) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename) noexcept(false);

    protected:
        //--- protected methods ---
        std::string genMappedData(Pixels &palette, const Pixels &pixels) const noexcept(false);
        std::string genTruecolorData(const Pixels &pixels) const noexcept(false);
        std::string genMonoData(const Pixels &pixels) const noexcept(false);
        std::string genMappedRleData(Pixels &palette, const Pixels &pixels) const noexcept(false);
        std::string genTruecolorRleData(const Pixels &pixels) const noexcept(false);
        std::string genMonoRleData(const Pixels &pixels) const noexcept(false);
        Pixels loadMappedData(std::istream &is, const Header header) const noexcept(false);
        Pixels loadTruecolorData(std::istream &is, const Header header) const noexcept(false);
        Pixels loadMonoData(std::istream &is, const Header header) const noexcept(false);
        Pixels loadMappedRleData(std::istream &is, const Header header) const noexcept(false);
        Pixels loadTruecolorRleData(std::istream &is, const Header header) const noexcept(false);
        Pixels loadMonoRleData(std::istream &is, const Header header) const noexcept(false);

    private:
        //--- private properties ---
        uint8_t _colormap_type;
        ImageType _image_type;
        uint16_t _colormap_offset;
        uint16_t _colormap_length;
        uint8_t _colormap_entry_size;
        uint16_t _x_origin;
        uint16_t _y_origin;
        uint8_t _depth;
        uint8_t _image_descriptor;
        std::string _image_id;
        bool _version2;
        bool _greyscale;
    };
}
