cmake_minimum_required(VERSION 4.0)

if (NOT DEFINED OBJC_PREFIX)
    string(RANDOM LENGTH 5 ALPHABET ABCDEFGHIJKLMNOPQRSTUVWXYZ OBJC_PREFIX)
endif ()

# this is the one we will install
add_library(PluginAfterEffects MODULE)

if (MSVC)
    set(${PROJECT_NAME}_OUTPUT_NAME "${CODEC_NAME}Plugin")
elseif (APPLE)
# macOS plugins have a common extension, so identify the host in the name
    set(${PROJECT_NAME}_OUTPUT_NAME "${CODEC_NAME}AfterEffects")
endif()

target_sources(PluginAfterEffects
    PRIVATE
        output_module.cpp
        output_module.cpp
        output_module.h
        output_module.cpp
        ui.h
        $<$<PLATFORM_ID:Windows>:win/output_module.rc>
        $<$<PLATFORM_ID:Windows>:win/output_module.rc>
        $<$<PLATFORM_ID:Windows>:win/output_module_temp.rc>
        $<$<PLATFORM_ID:Windows>:win/resource.h>
        $<$<PLATFORM_ID:Windows>:win/ui.cpp>
        $<$<PLATFORM_ID:Darwin>:mac/ui.mm>
        $<$<PLATFORM_ID:Darwin>:mac/FNDControlViewController.m>
        $<$<PLATFORM_ID:Darwin>:mac/FNDObjC.h>
        $<$<PLATFORM_ID:Darwin>:mac/FNDControlViewController.h>
        $<$<PLATFORM_ID:Darwin>:mac/PkgInfo>
        $<$<PLATFORM_ID:Darwin>:${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_OUTPUT_NAME}.rsrc>
)

if (NOT DEFINED IDENTIFIER_PREFIX)
    message(FATAL_ERROR "Parent must define IDENTIFIER_PREFIX")
endif ()
if (NOT DEFINED CODEC_NAME)
    message(FATAL_ERROR "Parent must define CODEC_NAME")
endif ()
if (NOT DEFINED INSTALL_DESTINATION)
    message(FATAL_ERROR "Parent must define INSTALL_DESTINATION")
endif ()
if (NOT DEFINED Foundation_OBJC_PREFIX)
string(RANDOM LENGTH 5 ALPHABET ABCDEFGHIJKLMNOPQRSTUVWXYZ
        Foundation_OBJC_PREFIX)
endif ()

set(${PROJECT_NAME}_OSX_BUNDLE_IDENTIFIER ${IDENTIFIER_PREFIX}.aex)

if (APPLE)
    set_property(SOURCE mac/ui.mm mac/FNDControlViewController.m
                    APPEND_STRING
                    PROPERTY COMPILE_FLAGS "-fobjc-arc"
    )
    set_property(SOURCE mac/ui.mm mac/FNDControlViewController.m
                    APPEND
                    PROPERTY COMPILE_DEFINITIONS FOUNDATION_OBJC_PREFIX=${Foundation_OBJC_PREFIX} BUNDLE_GUI_IDENTIFIER=@\"${${PROJECT_NAME}_OSX_BUNDLE_IDENTIFIER}\"
    )
# Required for AE to load the plugin at all
    set_source_files_properties(mac/PkgInfo
                                PROPERTIES MACOSX_PACKAGE_LOCATION ""
    )
endif (APPLE)

find_library(AppKit AppKit)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(PluginAfterEffects
    PRIVATE
        AdobeAfterEffectsSDK
        Utility
        BaseCodec
        HapCodec
        MediaSession
        nlohmann_json::nlohmann_json
        $<$<PLATFORM_ID:Windows>:StackWalker>
        $<$<PLATFORM_ID:Darwin>:${AppKit}>
)

set_target_properties(PluginAfterEffects
        PROPERTIES
            OUTPUT_NAME "${${PROJECT_NAME}_OUTPUT_NAME}"
)

if (MSVC)
    set_target_properties(PluginAfterEffects
        PROPERTIES
            SUFFIX ".aex"
    )
endif(MSVC)

if (APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/SettingsViewController.xib.in ${CMAKE_BINARY_DIR}/SettingsViewController.xib)
endif (APPLE)

if (APPLE AND NOT XCODE)
    target_sources(PluginAfterEffects
        PRIVATE
            ${CMAKE_BINARY_DIR}/SettingsViewController.nib)

    find_program(APPLE_IBTOOL ibtool)
    if (APPLE_IBTOOL)
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/SettingsViewController.nib
                            COMMAND ${APPLE_IBTOOL} --errors --warnings --notices --module "${PROJECT_NAME}" --target-device mac --output-format "human-readable-text" --compile "${CMAKE_BINARY_DIR}/SettingsViewController.nib" "${CMAKE_BINARY_DIR}/SettingsViewController.xib"
                            COMMENT "Creating macOS interface file"
                            DEPENDS ${CMAKE_BINARY_DIR}/SettingsViewController.xib
                            VERBATIM)
    else ()
        message(FATAL_ERROR "Can't find ibtool to build macOS interface file.")
    endif ()
elseif (APPLE)
    target_sources(PluginAfterEffects
        PRIVATE
            ${CMAKE_BINARY_DIR}/SettingsViewController.xib)
endif ()

if (APPLE)
    find_program(APPLE_RESOURCE Rez)
    if (APPLE_RESOURCE)
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_OUTPUT_NAME}.rsrc
                            COMMAND ${APPLE_RESOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/output_module.r -define __MACH__ -define CODEC_NAME="${CODEC_NAME}" -useDF -i "$<JOIN:$<TARGET_PROPERTY:AdobeAfterEffectsSDK,INTERFACE_INCLUDE_DIRECTORIES>,;-i;>" -o ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_OUTPUT_NAME}.rsrc
                            COMMENT "Creating macOS resource file"
                            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/output_module.r AdobeAfterEffectsSDK
                            VERBATIM
                            COMMAND_EXPAND_LISTS)
    else (APPLE_RESOURCE)
        message(FATAL_ERROR "Can't find Rez to build macOS resource file.")
    endif (APPLE_RESOURCE)
endif (APPLE)

set_target_properties(PluginAfterEffects
    PROPERTIES
	    BUNDLE TRUE
	    MACOSX_BUNDLE_GUI_IDENTIFIER ${${PROJECT_NAME}_OSX_BUNDLE_IDENTIFIER}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/mac/MacOSXBundleInfo.plist.in
)
if (APPLE)
# These could be a RESOURCE property on the target, however CMake gives a misleading warning about RESOURCE properties
    set_source_files_properties(
        ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_OUTPUT_NAME}.rsrc
        PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    if (XCODE)
        set_source_files_properties(
            ${CMAKE_BINARY_DIR}/SettingsViewController.xib
            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    else (XCODE)
        set_source_files_properties(
            ${CMAKE_BINARY_DIR}/SettingsViewController.nib
            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    endif (XCODE)
endif (APPLE)

install(
  TARGETS PluginAfterEffects
  RUNTIME
  BUNDLE DESTINATION ${INSTALL_DESTINATION}
  LIBRARY DESTINATION ${INSTALL_DESTINATION}
  COMPONENT plugin
)
