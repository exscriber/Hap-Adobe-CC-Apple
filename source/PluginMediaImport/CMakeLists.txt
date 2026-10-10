cmake_minimum_required(VERSION 4.0)

add_library(PluginMediaImport MODULE)

if(MSVC)
    target_compile_definitions(PluginMediaImport PRIVATE 
        FILE_IMPORT_FOUR_CC=${FILE_IMPORT_FOUR_CC}L)
endif()

if (MSVC)
    set(MODULE_NAME "${CODEC_NAME}ImportPlugin")
elseif (APPLE)
    set(MODULE_NAME "${CODEC_NAME}MediaImport")
endif()

if(APPLE)
    # Generated Resource file
    set(RES_FILE_GEN ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.rsrc)
    set_source_files_properties(
        ${RES_FILE_GEN}
        PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

target_sources(PluginMediaImport
    PRIVATE
        premiereImporter.cpp
        premiereImporter.hpp
        async_importer.cpp
        async_importer.hpp
        $<$<PLATFORM_ID:Windows>:Import.rc>
        $<$<PLATFORM_ID:Darwin>:${RES_FILE_GEN}>
)

find_library(Foundation Foundation)

target_link_libraries(PluginMediaImport
    PRIVATE
        AdobePremiereProSDK
        Utility
        BaseCodec
        HapCodec
        MediaSession
        $<$<PLATFORM_ID:Windows>:StackWalker>
        $<$<PLATFORM_ID:Darwin>:${Foundation}>
)

if (APPLE)
    find_program(APPLE_REZ Rez REQUIRED)
    add_custom_command(
        OUTPUT ${RES_FILE_GEN}
        COMMAND ${APPLE_REZ}
            ${CMAKE_CURRENT_SOURCE_DIR}/Import.r
            -define __MACH__
            -define FILE_IMPORT_FOUR_CC=${FILE_IMPORT_FOUR_CC}
            -useDF
            -o "${RES_FILE_GEN}"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Import.r
        COMMENT "Building resource ${RES_FILE_GEN}"
        VERBATIM
    )
endif (APPLE)

set_target_properties(PluginMediaImport PROPERTIES
    OUTPUT_NAME "${MODULE_NAME}"
    SUFFIX ".prm"

    BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER_PREFIX}.import
)

install(
  TARGETS PluginMediaImport
  RUNTIME
  BUNDLE DESTINATION ${INSTALL_DESTINATION}
  LIBRARY DESTINATION ${INSTALL_DESTINATION}
  COMPONENT plugin
)
