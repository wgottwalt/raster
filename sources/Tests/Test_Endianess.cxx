#include <iostream>
#include <string>
#include "Common/Endian.hxx"

namespace E = Common::Endian;

template <Common::Concept::Number T>
void print(const T val)
{
    std::cout << std::hex << "0x" << val << " toBE 0x" << E::toBE(val) << std::endl;
}

int32_t main()
{
    const uint64_t val64 = 0x1122334455667788ull;
    const uint32_t val32 = 0x11223344;
    const uint16_t val16 = 0x1122;

    print(val64);
    print(val32);
    print(val16);

    return 0;
}
