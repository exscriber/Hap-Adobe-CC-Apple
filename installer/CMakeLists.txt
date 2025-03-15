cmake_minimum_required(VERSION 3.31)

set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

# macOS installer specific settings
if (APPLE)
    set (CPACK_GENERATOR productbuild)
    set (CPACK_PACKAGING_INSTALL_PREFIX "/")
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
       set(CMAKE_INSTALL_PREFIX "/" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (NOT CPACK_RESOURCE_FILE_README)
        message(AUTHOR_WARNING "CPACK_RESOURCE_FILE_README is not set. CMake's default will be used.")
    endif ()
    if (NOT CPACK_RESOURCE_FILE_WELCOME)
        set (CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_LIST_DIR}/Welcome.txt)
    endif ()
    # User can opt to provide a different name outside this file
    if (NOT CPACK_PRODUCTBUILD_IDENTITY_NAME)
        set (CPACK_PRODUCTBUILD_IDENTITY_NAME "Developer ID Installer")
    endif ()
    
    # Default is Darwin, not useful for generated file names:
    set (CPACK_SYSTEM_NAME macOS)
endif (APPLE)
# NSIS component specific settings
set(CPACK_NSIS_plugin_INSTALL_DIRECTORY "$PROGRAMFILES64\\\\Adobe\\\\Common\\\\Plug-ins\\\\7.0\\\\MediaCore\\\\${CODEC_NAME}")
# set(CPACK_INSTALL_COMMANDS, "magic command to get plugin directory and copy to it")
# currently doing this by overriding template
# TODO: don't override template
# override modules path so we can change the nsis template
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})


# generic installer framework
# This must always be after CPACK settings
include(CPack)
# components to include to installer
cpack_add_component(plugin
    DISPLAY_NAME "Plugin"
    DESCRIPTION "Plugins for Adobe Creative Cloud"
    REQUIRED
)
