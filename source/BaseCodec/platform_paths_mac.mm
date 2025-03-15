#include <Foundation/Foundation.h>

#include "platform_paths.hpp"

fdn::PathType fdn::getConfigurationPath()
{
    fdn::PathType path;
    @autoreleasepool {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSLocalDomainMask, YES);
        NSString *applicationSupportDirectory = [paths firstObject];
        NSString *configurationPath = [NSString stringWithFormat:@"%@%@%s",
                                                applicationSupportDirectory,
                                                @"/Adobe/Common/Plug-ins/7.0/MediaCore/",
                                                CODEC_NAME];
        path = [[configurationPath stringByExpandingTildeInPath] cStringUsingEncoding:NSUTF8StringEncoding];
        
    }
    return path;
}
