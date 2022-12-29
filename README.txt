Convert image to match color palette.

How it works
 Swaps every pixel with the closest one from the palette,
 or blends it by the blend factor if it is not equal to 1.

Prerequisites
 - c++17 compiler
 - Boost GIL library

Build
 With make
  make

 With CMake
  mkdir <build_dir> && cd <build_dir>
  cmake -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .

Usage
 image_match_palette <input_path.png> <output_path.png> <palette_path> [blend 0.0-1.0, default 1.0]

 Example
  image_match_palette ../palatte-gruvbox.txt ./in.png ./out.png 0.5

Notes
 - Currently 8-bit RGB PNG file type is hard coded
