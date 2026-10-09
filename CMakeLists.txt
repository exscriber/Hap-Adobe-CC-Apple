cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version")

project(HapCodecPlugin)

set(IDENTIFIER_PREFIX org.hapcommunity.HapCodecPlugin)
set(CODEC_NAME HAP)
set(FILE_IMPORT_FOUR_CC 0x48415000L)    # hex for HAP\0, needed as unique importer id so After Effects picks it up as an importer
set(CODEC_NAME_WITH_HEX_SIZE_PREFIX "\"\\x07HAP\\0\\0\\0\\0\"")    # !!! hack - size must be 0x7 atm, and padded to 7-bytes- we're not using the recommended resource builder step that compiles the correct sizes around this

add_subdirectory(external/Adobe)
add_subdirectory(source/BaseCodec)
add_subdirectory(source/HapCodec)
add_subdirectory(source/MediaSession)

if (APPLE)
    set (INSTALL_DESTINATION "Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/${CODEC_NAME}")
else ()
    set (INSTALL_DESTINATION .)
endif ()

add_definitions(-DCODEC_NAME="${CODEC_NAME}")
add_subdirectory(source/Utility)
add_subdirectory(source/PluginAfterEffects)
add_subdirectory(source/PluginPremierePro)

include(installer/CodecInstaller.cmake)
add_subdirectory(installer)
