#ifndef PENROSE_ASSETS_ASSET_MANAGER_HPP
#define PENROSE_ASSETS_ASSET_MANAGER_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Runnable.hpp>

namespace Penrose {

    class ResourceSet;

    template<typename T>
    concept IsAsset = std::is_base_of<Asset, T>::value;

    class AssetManager : public Resource, public Initializable, public Runnable {
    public:
        explicit AssetManager(ResourceSet *resources);
        ~AssetManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        void run() override;
        void stop() override;

        void enqueue(const std::string &asset);

        [[nodiscard]] bool isLoaded(const std::string &asset) const;

        void unload(const std::string &asset);

        template<IsAsset T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetAsset(const std::string &asset, bool wait = false) const;

        template<IsAsset T>
        [[nodiscard]] std::shared_ptr<T> getAsset(const std::string &asset) const;

    private:
        enum class LoadingState {
            Pending,
            Loaded,
            Failed
        };

        struct Entry {
            LoadingState state;
            std::shared_ptr<Asset> ptr;
        };

        Lazy<AssetDictionary> _assetDictionary;
        Lazy<AssetLoader> _assetLoader;
        Lazy<Log> _log;

        std::unordered_map<std::string, Entry> _assets;

        std::optional<std::jthread> _loadingThread;
        std::mutex _loadingMutex;
        std::queue<std::string> _loadingQueue;

        void tryLoadAsset(const std::string &asset);
        [[nodiscard]] std::optional<std::shared_ptr<Asset>> tryGetAsset(const std::string &asset, bool wait) const;
    };
}

#include "AssetManager.inl"

#endif // PENROSE_ASSETS_ASSET_MANAGER_HPP
