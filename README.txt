Convert image to match color palette.

How it works
 Swaps every pixel with the closest one from the palette,
 or blends it by the blend factor if it is not equal to 1.

Prerequisites
 - c++17 compiler
 - Boost GIL library

Build
 mkdir <build_dir> && cd <build_dir>
 cmake -DCMAKE_BUILD_TYPE=Debug ..
 cmake --build .

Usage
 <build_dir>/main <pallete_path> <input_path.png> <output_path.png> <blend 0.0-1.0>

 Example
  ./main ../palatte-gruvbox.txt ./in.png ./out.png 0.75

Notes
 - Currently 8-bit png file type is hard coded
