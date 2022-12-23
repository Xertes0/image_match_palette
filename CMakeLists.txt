cmake_minimum_required(VERSION 3.25)

# Boost gil does not compile with c++20
set(CMAKE_CXX_STANDARD 17)

project(image_palette)

find_package(Threads)

add_executable(main main.cc)

target_link_libraries(
    main PRIVATE
    Threads::Threads # for parallelism
    png # libpng
)
