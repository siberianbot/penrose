#ifndef PENROSE_ASSETS_UTILS_HPP
#define PENROSE_ASSETS_UTILS_HPP

#include <filesystem>
#include <map>
#include <string>

namespace Penrose {

    struct IndexFileEntry {
        std::filesystem::path path;
        bool preload;
    };

    using IndexFile = std::map<std::string, IndexFileEntry>;

    IndexFile readIndexFile(std::filesystem::path &&rootDir);
}

#endif // PENROSE_ASSETS_UTILS_HPP
