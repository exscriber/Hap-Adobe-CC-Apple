cmake_minimum_required(VERSION 4.0)

if (NOT DEFINED OBJC_PREFIX)
    string(RANDOM LENGTH 5 ALPHABET ABCDEFGHIJKLMNOPQRSTUVWXYZ OBJC_PREFIX)
endif ()

# this is the one we will install
add_library(PluginAfterEffects MODULE)

if(MSVC)
    target_compile_definitions(PluginAfterEffects PRIVATE 
        CODEC_NAME_WITH_HEX_SIZE_PREFIX=${CODEC_NAME_WITH_HEX_SIZE_PREFIX})
endif()

if (MSVC)
    set(MODULE_NAME "${CODEC_NAME}Plugin")
elseif (APPLE)
# macOS plugins have a common extension, so identify the host in the name
    set(MODULE_NAME "${CODEC_NAME}AfterEffects")
endif()

if(APPLE)
    # Generated PkgInfo file
    set(PKG_TYPE "AEgx")
    set(PKG_SIGN "FXTC")
    set(PKG_FILE_GEN ${CMAKE_CURRENT_BINARY_DIR}/PkgInfo)

    # Generated Resource file
    set(RES_FILE_GEN ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.rsrc)
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
        $<$<PLATFORM_ID:Darwin>:${PKG_FILE_GEN}>
        $<$<PLATFORM_ID:Darwin>:${RES_FILE_GEN}>
)

if (APPLE)
    # 
    set(AE_BUNDLE_IDENTIFIER ${IDENTIFIER_PREFIX}.aex)

    # ObjC files
    set_source_files_properties(mac/ui.mm mac/FNDControlViewController.m
    PROPERTIES
        COMPILE_OPTIONS "-fobjc-arc"
        COMPILE_DEFINITIONS "OBJC_PREFIX=${OBJC_PREFIX};BUNDLE_GUI_IDENTIFIER=@\"${AE_BUNDLE_IDENTIFIER}\""
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
            OUTPUT_NAME "${MODULE_NAME}"
)

if (MSVC)
    set_target_properties(PluginAfterEffects
        PROPERTIES
            SUFFIX ".aex"
    )
endif(MSVC)

if (APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/SettingsViewController.xib.in ${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.xib)
    if (XCODE)
        target_sources(PluginAfterEffects PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.xib)
    else ()
        find_program(APPLE_IBTOOL ibtool REQUIRED)
        set(NIB_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.nib")
        add_custom_command(
            OUTPUT ${NIB_OUTPUT}
            COMMAND ${APPLE_IBTOOL}
                --errors --warnings --notices
                --module "${PROJECT_NAME}"
                --target-device mac
                --output-format "human-readable-text"
                --compile "${NIB_OUTPUT}" "${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.xib"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.xib"
            COMMENT "Building UI interface ${NIB_OUTPUT}"
            VERBATIM
        )
        target_sources(PluginAfterEffects PRIVATE "${NIB_OUTPUT}")
    endif()
endif (APPLE)

if (APPLE)
    find_program(APPLE_REZ Rez REQUIRED)
    add_custom_command(
        OUTPUT ${RES_FILE_GEN}
        COMMAND ${APPLE_REZ}
            ${CMAKE_CURRENT_SOURCE_DIR}/PiPL.r
            -define __MACH__
            -define CODEC_NAME="${CODEC_NAME}"
            -useDF
            -i "$<JOIN:$<TARGET_PROPERTY:AdobeAfterEffectsSDK,INTERFACE_INCLUDE_DIRECTORIES>,;-i;>"
            -o "${RES_FILE_GEN}"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/PiPL.r AdobeAfterEffectsSDK
        COMMENT "Building resource ${RES_FILE_GEN}"
        VERBATIM
        COMMAND_EXPAND_LISTS
    )
    add_custom_command(
        OUTPUT ${PKG_FILE_GEN}
        COMMAND ${CMAKE_COMMAND} -E echo_append "${PKG_TYPE}${PKG_SIGN}" > "${PKG_FILE_GEN}"
        COMMENT "Generating PkgInfo"
        VERBATIM
    )
endif (APPLE)

set_target_properties(PluginAfterEffects
    PROPERTIES
	    BUNDLE TRUE
	    MACOSX_BUNDLE_GUI_IDENTIFIER ${AE_BUNDLE_IDENTIFIER}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/mac/Info.plist
)

if (APPLE)
    set_source_files_properties(${PKG_FILE_GEN}
        PROPERTIES MACOSX_PACKAGE_LOCATION "")

    # These could be a RESOURCE property on the target, however CMake gives a misleading warning about RESOURCE properties
    set_source_files_properties(
        ${RES_FILE_GEN}
        ${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.xib
        ${CMAKE_CURRENT_BINARY_DIR}/SettingsViewController.nib
        PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif (APPLE)

install(
  TARGETS PluginAfterEffects
  RUNTIME
  BUNDLE DESTINATION ${INSTALL_DESTINATION}
  LIBRARY DESTINATION ${INSTALL_DESTINATION}
  COMPONENT plugin
)
