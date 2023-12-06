#ifndef PENROSE_ASSETS_ASSET_INDEX_HPP
#define PENROSE_ASSETS_ASSET_INDEX_HPP

#include <atomic>
#include <filesystem>
#include <map>
#include <optional>
#include <string>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class AssetIndex final: public Resource<AssetIndex> {
    public:
        enum class State {
            Unloaded,
            Loading,
            Loaded,
            Failed
        };

        struct Entry {
            std::filesystem::path path;
            State state;
            std::weak_ptr<Asset> instance;
        };

        explicit AssetIndex(const ResourceSet *resources);
        ~AssetIndex() override = default;

        void add(std::string_view &&asset, std::filesystem::path &&path);

        void remove(std::string_view &&asset);
        void removeAll();

        void markUnloaded(std::string_view &&asset) noexcept;
        void markLoading(std::string_view &&asset) noexcept;
        void markLoaded(std::string_view &&asset, std::shared_ptr<Asset> &&instance) noexcept;
        void markFailed(std::string_view &&asset) noexcept;

        [[nodiscard]] std::optional<Entry> tryGet(std::string_view &&asset) const noexcept;

    private:
        struct InternalEntry {
            std::filesystem::path path;
            std::atomic<State> state;
            std::atomic<std::shared_ptr<Asset>> instance;
        };

        ResourceProxy<Log> _log;

        std::map<std::string, InternalEntry> _entries;
    };
}

#endif // PENROSE_ASSETS_ASSET_INDEX_HPP
