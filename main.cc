#include <algorithm>
#include <cmath>
#include <execution>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <vector>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/png/tags.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/typedefs.hpp>

namespace gil = boost::gil;
using image_t = gil::rgb8_image_t;
using pixel_t = gil::rgb8_pixel_t;
using channel_t = std::decay_t<decltype(pixel_t{}[0])>;

std::vector<pixel_t>
load_palette(std::filesystem::path const& path)
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

        pixel_t pixel {
            static_cast<channel_t>((hex & 0xFF'00'00U) >> 16),
            static_cast<channel_t>((hex & 0x00'FF'00U) >> 8),
            static_cast<channel_t>( hex & 0x00'00'FFU)
        };

        palette.emplace_back(std::move(pixel));
    }

    return palette;
}

int
main([[maybe_unused]] int argc, char** argv)
{
    std::cout << "Usage: ./main <input_path.png> <output_path.png> <palette_path> [blend 0.0-1.0, default 1.0]\n";
    std::cout << "Palette format: std::hex valid values separated by new line\n";

    double blend = 1.0;
    if(argc == 5) {
        blend = std::stod(argv[4]);
    } else if(argc != 4) {
        std::cerr << "Invalid argument count. Terminating...\n";
        return 1;
    }

    std::filesystem::path in_path{argv[1]};
    std::filesystem::path out_path{argv[2]};
    std::filesystem::path palette_path{argv[3]};

    std::cout << "Using:\n Input: "<<in_path<<"\n Output: "<<out_path<<"\n Palette: "<<palette_path<<"\n Blend: "<<blend<<'\n';

    auto palette = load_palette(palette_path);
    std::cout << "Loaded "<<palette.size()<<" colors from palette\n";

    ////

    gil::rgb8_image_t image{};
    gil::read_image(in_path.native(), image, gil::png_tag{});

    auto view = gil::view(image);
    std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&palette, blend](auto& pixel){
        pixel_t closest = palette[0];
        std::int32_t min_offset = std::numeric_limits<std::int32_t>::max();
        for(pixel_t color : palette) {
            std::int32_t offset =
                std::abs(static_cast<std::int32_t>(pixel[0]) - static_cast<std::int32_t>(color[0])) +
                std::abs(static_cast<std::int32_t>(pixel[1]) - static_cast<std::int32_t>(color[1])) +
                std::abs(static_cast<std::int32_t>(pixel[2]) - static_cast<std::int32_t>(color[2]));
            if(offset < min_offset) {
                min_offset = offset;
                closest = color;
            }
        }

        if(blend == 1.0) {
            pixel = closest;
        } else {
            double diff_r = static_cast<double>(pixel[0]) - static_cast<double>(closest[0]);
            double diff_g = static_cast<double>(pixel[1]) - static_cast<double>(closest[1]);
            double diff_b = static_cast<double>(pixel[2]) - static_cast<double>(closest[2]);

            pixel[0] = static_cast<channel_t>(static_cast<double>(pixel[0]) - (diff_r * blend));
            pixel[1] = static_cast<channel_t>(static_cast<double>(pixel[1]) - (diff_g * blend));
            pixel[2] = static_cast<channel_t>(static_cast<double>(pixel[2]) - (diff_b * blend));
        }
    });

    gil::write_view(out_path.native(), view, gil::png_tag{});

    return 0;
}
