cmake_minimum_required(VERSION 4.0)

add_library(Utility
    string_conversion.hpp
    string_conversion.cpp
)

target_include_directories(Utility
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

find_library(CoreFoundation CoreFoundation)

target_link_libraries(Utility
    $<$<PLATFORM_ID:Darwin>:${CoreFoundation}>
)
