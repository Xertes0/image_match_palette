#include <algorithm>
#include <cmath>
#include <cstdio>
#include <execution>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <sstream>
#include <string_view>
#include <vector>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/png/tags.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/typedefs.hpp>

namespace gil = boost::gil;
using image_t = gil::rgb8_image_t;
using pixel_t = gil::rgb8_pixel_t;

// using c++17 so can't use std::filesystem::path
std::vector<pixel_t>
load_palette(char const* path)
{
    std::ifstream palette_file{path, std::ios_base::in};

    palette_file.seekg(0, std::ios_base::end);
    auto line_count = palette_file.tellg();
    palette_file.seekg(0, std::ios_base::beg);

    std::vector<pixel_t> palette{};
    palette.reserve(line_count);

    while(!palette_file.fail()) {
        std::uint32_t hex{};
        palette_file >> std::hex >> hex;

        pixel_t pixel{};
        pixel[0] = (hex & 0xFF'00'00U) >> 16;
        pixel[1] = (hex & 0x00'FF'00U) >> 8;
        pixel[2] =  hex & 0x00'00'FFU;

        palette.emplace_back(std::move(pixel));
    }

    return palette;
}

int
main([[maybe_unused]] int argc, char** argv)
{
    std::fprintf(stderr, "Usage: ./main <palette_path> <input_path.png> <output_path.png>\n");
    std::fprintf(stderr, "Palette format: rgb in hex in each line\n");
    std::fprintf(stderr, "Hope you entered it right 'couse i sure ain't checking it\n");

    auto palette = load_palette(argv[1]);
    std::printf("Loaded %zu colors from palette\n", palette.size());

    gil::rgb8_image_t image{};
    gil::read_image(argv[2], image, gil::png_tag{});

    auto view = gil::view(image);
    std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&palette](auto& pixel){
        ::pixel_t closest = palette[0];
        std::int32_t min_offset = std::numeric_limits<std::int32_t>::max();
        for(::pixel_t color : palette) {
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
    });

    gil::write_view(argv[3], view, gil::png_tag{});

    return 0;
}
