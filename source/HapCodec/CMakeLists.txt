cmake_minimum_required(VERSION 3.31)

project(HapCodec)

add_library(HapCodec
        hap.c
        hap.h
        ImageMath.c
        ImageMath.h
        YCoCg.c
        YCoCg.h
        YCoCgDXT.cpp
        YCoCgDXT.h
        HapEncoder.cpp
        HapEncoder.hpp
        SquishTextureConverter.cpp
        TextureConverter.hpp
)

target_include_directories(HapCodec
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

find_package(Snappy CONFIG REQUIRED)
find_package(unofficial-libsquish CONFIG REQUIRED)
find_library(Accelerate Accelerate)

target_link_libraries(HapCodec
    BaseCodec
    Snappy::snappy
    unofficial::libsquish::squish
    $<$<PLATFORM_ID:Darwin>:${Accelerate}>
)
