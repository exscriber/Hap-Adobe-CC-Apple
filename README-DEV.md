# Codec Foundation for Adobe CC

This is the community-supplied foundation for creating plugins to Adobe CC 2018 and 2019.

Development of this plugin was sponsored by
 - [disguise](http://disguise.one), makers of the disguise show production software and hardware.
 - [10bitFX](http://notch.one), creators of the Notch VFX software

Principal contributors to this plugin are

-  Greg Bakker (gbakker@gmail.com)
-  Richard Sykes
-  [Tom Butterworth](http://kriss.cx/tom)
-  [Nick Zinovenko](https://github.com/exscriber)

Thanks to Tom Butterworth for creating the Hap codec and Vidvox for supporting that development.

Please see [LICENSE](LICENSE) for the licenses of this plugin and the components used to create it.

# Development

The following information is for developers who wish to contribute to the project.

## Prerequisites

### compiler toolchain

You'll need a compiler environment appropriate to your operating system.
The current plugin has been developed on:

-  macOS with Xcode 16

### CMake

CMake creates the build system for the supported target platforms. This project requires version 3.31 or later  
[https://cmake.org/install](https://cmake.org/install)

### vcpkg

Dependencies handled by vcpkg  
<https://vcpkg.io>

<!-- ### NSIS

NSIS is required for win32 installer builds  
[http://nsis.sourceforge.net](http://nsis.sourceforge.net) -->

### Adobe SDKs

Following SDKs are required from Adobe  
<https://developer.adobe.com/console>

| SDK                     | Location                       | Guide                                 |
|-------------------------|--------------------------------|---------------------------------------|
| Premiere Pro Plugin SDK | external/Adobe/PremiereProSDK  | https://ppro-plugins.docsforadobe.dev |
| AfterEffects Plugin SDK | external/Adobe/AfterEffectsSDK | https://ae-plugins.docsforadobe.dev   |


##  Building

<!-- ### win64

First create a build directory at the top level, and move into it

    mkdir build
    cd build

Invoke cmake to create a Visual Studio .sln

    cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..

This should create HapEncoder.sln in the current directory. Open it in Visual Studio:

    HapEncoder.sln

The encoder plugin (.prm) is created by building all.
The installer executable is made by building the PACKAGE target, which is excluded from the regular build. -->

### macOS

Preparations:

    brew install cmake vcpkg
    git clone https://github.com/microsoft/vcpkg "$HOME/vcpkg"
    echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.zshrc

CMake configuration for VScode:

    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    }

CMake configuration for Xcode:

    cmake --toolchain ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -B Xcode -G Xcode

To create an installer (requires Apple Developer Program membership for signing)

    cpack

To create an unsigned installer for development testing (do not distribute unsigned installers)

    cpack -D CPACK_PRODUCTBUILD_IDENTITY_NAME=

The installer is created in the Release directory.

#### Notarizing macOS Installers

Installers for macOS **must** be notarized by Apple. If you skip this step, users will not be able to install your software. Because it requires prior setup it is not automated. Follow the instructions under "Upload Your App to the Notarization Service" in Apple's [Customizing the Notarization Workflow](https://developer.apple.com/documentation/xcode/notarizing_your_app_before_distribution/customizing_the_notarization_workflow?language=objc) guide.

### Plugin Configuration

The codec_registration library provides a means of obtaining configuration information both for itself and for your own codecs.

Configuration information is stored in json format, and on windows is loaded from

    C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\HAP\config.json

and on macos is loaded from

    /Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/HAP/config.json

a sample config.json file on macos is

    {
        "logging": {
            "threshold": "debug",
            "path": "/Users/[Username]/Desktop"
        },
        "exporter": {
           "initialWorkers": 1,
           "maxWorkers": -1
        }
    }

meaning that maximum logging is enabled, the exporters start with 1 worker thread and use a maximum of <number of cores on your machine> workers.

Your own configuration may be added alongside these. It is available as parsed json, from which you can serialise. Please see external/json for details.

Assuming you have implemented an nlohmann::json serializer for your configuration information, you could obtain it in your plugin with

    #include "config.hpp"

    ...

    YourConfiguration config;
    fdn::config().at("exporter").get_to(config);
    
### Logging

The foundation also supplies a thread-safe logging facility that you may use in your own code. This is available in the codec_registration library.

Please see logging.hpp for details, but after #including config.hpp you may use the various FDN_<log level> macros.

    FDN_INFO("we are now doing this", b, "something", 17.0);
    FDN_INFO("we are now doing that", d);

    FDN_DEBUG("testvar7:", testvar7);
    FDN_WARN("disk space seems low");
    FDN_ERROR("ran out of disk space");
    FDN_FATAL("heap corruption detected; exiting");

Logging information is written to the debug output of an attached debugger, and additionally to a logfile as described in the Plugin Configuration section above.
