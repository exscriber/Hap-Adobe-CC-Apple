cmake_minimum_required(VERSION 4.0)
project(AdobeSDK LANGUAGES C CXX)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/AfterEffectsSDK/Headers/AEConfig.h)
    message(WARNING "AfterEffectsSDK not found")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/PremiereProSDK/Headers/PrSDKEntry.h)
    message(WARNING "PremiereProSDK not found")
endif()

add_library(AdobePremiereProSDK INTERFACE)
target_include_directories(AdobePremiereProSDK
    SYSTEM INTERFACE
        PremiereProSDK/Headers
        PremiereProSDK/Utils
)

add_library(AdobeAfterEffectsSDK STATIC)
target_sources(AdobeAfterEffectsSDK
    PRIVATE
        AfterEffectsSDK/Util/AEGP_SuiteHandler.h
        AfterEffectsSDK/Util/AEGP_SuiteHandler.cpp
        AfterEffectsSDK/Util/MissingSuiteError.cpp
)
target_include_directories(AdobeAfterEffectsSDK
    SYSTEM PUBLIC
        AfterEffectsSDK/Headers
        AfterEffectsSDK/Headers/SP
        AfterEffectsSDK/Util
        AfterEffectsSDK/Resources
)
