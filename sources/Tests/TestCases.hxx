#pragma once

#include "Image/Image.hxx"

namespace TestCase
{
    static constexpr int64_t DefaultMinWidth = 320;
    static constexpr int64_t DefaultMinHeight = 240;
    static constexpr int64_t DefaultWidth = 1280;
    static constexpr int64_t DefaultHeight = 720;

    bool applyToImageCase00(Image::Base &pic);
}
