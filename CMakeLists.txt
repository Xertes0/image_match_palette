cmake_minimum_required(VERSION 3.25)

# Boost gil does not compile with c++20
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    add_compile_options(-Wall -Wextra -Werror -Wconversion -Wpedantic -Wshadow -fsanitize=address,leak,undefined)
    add_link_options(-fsanitize=address,leak,undefined)
endif()

project(image_palette)

find_package(Threads)

add_executable(main main.cc)

target_link_libraries(
    main PRIVATE
    Threads::Threads # for parallelism
    png # libpng
)
