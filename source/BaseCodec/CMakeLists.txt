cmake_minimum_required(VERSION 3.31)

if (APPLE)
    find_library(Foundation Foundation)
endif (APPLE)

# this is the one we will install
add_library(BaseCodec
        codec.hpp
        config.hpp
        config.cpp
        logging.hpp
        logging.cpp
        util.hpp
        util.cpp
        platform_paths.hpp
        $<$<PLATFORM_ID:Windows>:platform_paths_windows.cpp>
        $<$<PLATFORM_ID:Darwin>:platform_paths_mac.mm>
)

target_include_directories(BaseCodec
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

# used so we can look up install location from code, name log files, label dialog boxes
target_compile_definitions(BaseCodec
    PUBLIC CODEC_NAME="${CODEC_NAME}"
)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(BaseCodec
    PUBLIC
        nlohmann_json::nlohmann_json
    PRIVATE
        $<$<PLATFORM_ID:Windows>:StackWalker>
        $<$<PLATFORM_ID:Darwin>:${Foundation}>
)
