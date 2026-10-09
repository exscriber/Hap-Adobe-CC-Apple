cmake_minimum_required(VERSION 4.0)

# this is the one we will install
add_library(PluginPremierePro MODULE)

target_sources(PluginPremierePro
    PRIVATE
        configure.hpp
        export_settings.hpp
        premiereExporter.cpp
        premiereExporter.hpp
        premiereParams.cpp
        premiereParams.hpp
        prstring.cpp
        prstring.hpp
)

find_library(Foundation Foundation)

target_link_libraries(PluginPremierePro
    PRIVATE
        AdobePremiereProSDK
        Utility
        BaseCodec
        HapCodec
        MediaSession
        $<$<PLATFORM_ID:Windows>:StackWalker>
        $<$<PLATFORM_ID:Darwin>:${Foundation}>
)

if (MSVC)
    set_target_properties(PluginPremierePro
        PROPERTIES
            OUTPUT_NAME "${CODEC_NAME}Plugin"
            SUFFIX ".prm"
    )
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)
elseif (APPLE)
# macOS plugins have a common extension, so identify the host in the name
    set_target_properties(PluginPremierePro
        PROPERTIES
            OUTPUT_NAME "${CODEC_NAME}PremierePro"
    )
endif()

if (NOT DEFINED INSTALL_DESTINATION)
    message(FATAL_ERROR "Parent must define INSTALL_DESTINATION")
endif ()
if (APPLE)
if (NOT DEFINED IDENTIFIER_PREFIX)
message(FATAL_ERROR "Parent must define IDENTIFIER_PREFIX")
endif ()
endif (APPLE)

set_target_properties(PluginPremierePro
    PROPERTIES
	    BUNDLE TRUE
	    MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER_PREFIX}.pr
)

install(
  TARGETS PluginPremierePro
  RUNTIME
  BUNDLE DESTINATION ${INSTALL_DESTINATION}
  LIBRARY DESTINATION ${INSTALL_DESTINATION}
  COMPONENT plugin
)
