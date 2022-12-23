cmake_minimum_required(VERSION 3.25)
project(image_palette)

add_executable(main main.cc)
target_link_libraries(
    main PRIVATE
    png
)
