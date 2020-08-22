#include <iostream>
#include <string>
#include "Common/Concepts.hxx"
#include "Math/Vector4.hxx"

using V4f = Math::Vector4<float>;

template <Common::Concept::Number N>
void pv4(const Math::Vector4<N> &vec, const std::string &str = "")
{
    std::cout << str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << std::endl;
}

int32_t main()
{
    V4f vec1;
    V4f vec2(1);
    V4f vec3(4, 9, 16, 25);
    V4f vec4(vec3);

    pv4(vec1, "vec1: ");
    pv4(vec2, "vec2: ");
    pv4(vec3, "vec3: ");
    pv4(vec4, "vec4(&vec3): ");

    V4f vec5(std::move(vec4));
    pv4(vec5, "vec5(&&vec4): ");

    vec5 = vec2;
    pv4(vec5, "vec5 = &vec2: ");

    vec5 = std::move(vec3);
    pv4(vec5, "vec5 = &&vec3: ");

    return 0;
}
