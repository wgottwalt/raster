#pragma once

#include <cstdint>
#include <iosfwd>

namespace Compression
{
    class LZW16 {
    public:
        //--- public constructors ---
        LZW16();
        LZW16(const LZW16 &rhs) = delete;
        LZW16(LZW16 &&rhs) = delete;
        ~LZW16();

        //--- public operators ---
        LZW16 &operator=(const LZW16 &rhs) = delete;
        LZW16 &operator=(LZW16 &&rhs) = delete;

        //--- public methods ---
        uint64_t bytesRead() const;
        uint64_t bytesWritten() const;
        void reset();
        bool done() const;
        void encode(std::istream &is, std::ostream &os);
        void decode(std::istream &is, std::ostream &os);

    private:
        //--- private properties ---
        uint64_t _bytes_in;
        uint64_t _bytes_out;
        bool _done;
    };
}
