#include <iostream>
#include <string>
#include "Common/Concepts.hxx"
#include "Math/Vector2.hxx"

using V2f = Math::Vector2<float>;

template <Common::Concept::Number N>
void pv2(const Math::Vector2<N> &vec, const std::string &str = "")
{
    std::cout << str << vec.x << ", " << vec.y << std::endl;
}

int32_t main()
{
    V2f vec1;
    V2f vec2(1);
    V2f vec3(4, 9);
    V2f vec4(vec3);

    pv2(vec1, "vec1: ");
    pv2(vec2, "vec2: ");
    pv2(vec3, "vec3: ");
    pv2(vec4, "vec4: ");

    V2f vec5(std::move(vec4));
    pv2(vec5, "vec5: ");

    vec5 = vec2;
    pv2(vec5, "vec5 = vec2: ");

    vec5 = std::move(vec3);
    pv2(vec5, "vec5 = move vec3: ");

    return 0;
}
