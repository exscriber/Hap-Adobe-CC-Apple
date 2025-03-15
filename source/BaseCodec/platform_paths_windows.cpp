#include <ShlObj.h>

#include "platform_paths.hpp"

namespace fdn {

PathType getConfigurationPath()
{
    PathType path;
    PWSTR programFiles;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramFilesX64, 0, NULL, &programFiles);
    if (SUCCEEDED(hr))
    {
        path = programFiles;
        path = path / "Adobe" / "Common" / "Plug-Ins" / "7.0" / "MediaCore" / CODEC_NAME;
        CoTaskMemFree(programFiles);
        return path;
    }
    throw std::runtime_error("could not get program files path");
}

} // namespace fdn