cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version")

project(HapCodecPlugin)

set(IDENTIFIER_PREFIX org.hapcommunity.HapCodecPlugin)
set(CODEC_NAME HAP)

function(FourCC RESULT ASCII)
    string(HEX "${ASCII}" FOUR_CC_HEX)
    string(APPEND FOUR_CC_HEX "00000000")
    string(SUBSTRING ${FOUR_CC_HEX} 0 8 FOUR_CC_HEX)
    set(${RESULT} "0x${FOUR_CC_HEX}" PARENT_SCOPE)
endfunction()

# MediaCore drawtype - this unique fourcc is required by After Effects,
# and is not related to the filetype(s) supported by this MediaCore importer
FourCC(FILE_IMPORT_FOUR_CC HAP1)

# HACK: Win only - size must be 0x7 atm, and padded to 7-bytes
# TODO: get rid of this variable and PluginAfterEffects/win/output_module_temp.rc as well
# TODO: use PiPLtool.exe resource compiler from SDK to process *.r files on Win
set(CODEC_NAME_WITH_HEX_SIZE_PREFIX "\"\\x07HAP\\0\\0\\0\\0\"") 

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
