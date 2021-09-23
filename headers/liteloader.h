#pragma once
#include <string>
#ifdef LITELOADER_EXPORTS
#    define LIAPI __declspec(dllexport)
#else
#    define LIAPI __declspec(dllexport)
#endif
#define LITELOADER_VERSION "1.2.0"
#define LITELOADER_VERSION_NUMBER 14
typedef unsigned char uchar;

namespace loaderapi {
LIAPI std::string getLoaderVersion();
LIAPI unsigned short getLoaderVersionNum();
LIAPI bool           isDebugMode();
};  // namespace loaderapi
#define LITELOADER_VERSION_GITHUB LITELOADER_VERSION"-Ci-48996a3_1632310659"
