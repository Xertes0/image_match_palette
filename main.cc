#include <boost/gil/io/write_view.hpp>
#include <limits>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png/tags.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/io/png.hpp>

namespace gil = boost::gil;

struct color_wrapper
{
    color_wrapper(std::uint32_t val)
    {
        pixel[0] = (val & 0xFF'00'00) >> 16;
        pixel[1] = (val & 0x00'FF'00) >> 8;
        pixel[2] = (val & 0x00'00'FF) >> 0;
    };

    operator
    gil::rgb8_pixel_t()
    {
        return pixel;
    }

    gil::rgb8_pixel_t pixel;
};

std::array<color_wrapper, 37> colors
{
    0x1d2021,
    0x282828,
    0x32302f,
    0x3c3836,
    0x504945,
    0x665c54,
    0x7c6f64,
    0x928374,
    0x928374,
    0xf9f5d7,
    0xfbf1c7,
    0xf2e5bc,
    0xebdbb2,
    0xd5c4a1,
    0xbdae93,
    0xa89984,
    0xfb4934,
    0xb8bb26,
    0xfabd2f,
    0x83a598,
    0xd3869b,
    0x8ec07c,
    0xfe8019,
    0xcc241d,
    0x98971a,
    0xd79921,
    0x458588,
    0xb16286,
    0x689d6a,
    0xd65d0e,
    0x9d0006,
    0x79740e,
    0xb57614,
    0x076678,
    0x8f3f71,
    0x427b58,
    0xaf3a03,
};

int
main()
{
    gil::rgb8_image_t image{};
    gil::read_image("in.png", image, gil::png_tag{});
    auto view = gil::view(image);
    for(auto& pixel : view) {
        gil::rgb8_pixel_t closest = colors[0];
        std::int32_t min_offset = std::numeric_limits<std::int32_t>::max();
        for(gil::rgb8_pixel_t color : colors) {
            std::int32_t offset =
                std::abs(static_cast<std::int32_t>(pixel[0]) - static_cast<std::int32_t>(color[0])) +
                std::abs(static_cast<std::int32_t>(pixel[1]) - static_cast<std::int32_t>(color[1])) +
                std::abs(static_cast<std::int32_t>(pixel[2]) - static_cast<std::int32_t>(color[2]));
            if(offset < min_offset) {
                min_offset = offset;
                closest = color;
            }
        }
        pixel = closest;
    }
    gil::write_view("out.png", view, gil::png_tag{});
    return 0;
}
