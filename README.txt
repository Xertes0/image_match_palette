Convert image to match color palette.

How it works
 Swaps every pixel with the closest one from the palette.

Prerequisites
 - c++17 compiler
 - Boost GIL library

Build
 mkdir <build_dir> && cd <build_dir>
 cmake -DCMAKE_BUILD_TYPE=Debug ..
 cmake --build .

Usage
 <build_dir>/main <pallete_path> <input_path.png> <output_path.png>

Notes
 - Currently 8-bit png file type is hard coded
