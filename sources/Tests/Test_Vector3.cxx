#include <iostream>
#include <string>
#include "Common/Concepts.hxx"
#include "Math/Vector3.hxx"

using V3f = Math::Vector3<float>;

template <Common::Concept::Number N>
void pv3(const Math::Vector3<N> &vec, const std::string &str = "")
{
    std::cout << str << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
}

int32_t main()
{
    V3f vec1;
    V3f vec2(1);
    V3f vec3(4, 9, 16);
    V3f vec4(vec3);

    pv3(vec1, "vec1: ");
    pv3(vec2, "vec2: ");
    pv3(vec3, "vec3: ");
    pv3(vec4, "vec4: ");

    V3f vec5(std::move(vec4));
    pv3(vec5, "vec5: ");

    vec5 = vec2;
    pv3(vec5, "vec5 = vec2: ");

    vec5 = std::move(vec3);
    pv3(vec5, "vec5 = move vec3: ");

    return 0;
}
