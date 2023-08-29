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
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetDictionary;
    class DeviceContext;
    class ImageAssetFactory;
    class MeshAssetFactory;
    class ShaderAssetFactory;

    template<typename T>
    concept IsAsset = std::is_base_of<Asset, T>::value;

    class AssetManager : public Resource, public Initializable {
    public:
        explicit AssetManager(ResourceSet *resources);
        ~AssetManager() override = default;

        void init() override;
        void destroy() override;

        void queueImageLoading(const std::string &asset);
        void queueMeshLoading(const std::string &asset);
        void queueShaderLoading(const std::string &asset);

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
            AssetType type;
            LoadingState state;
            std::shared_ptr<Asset> ptr;
        };

        AssetDictionary *_assetDictionary;
        DeviceContext *_deviceContext;

        ImageAssetFactory *_imageAssetFactory;
        MeshAssetFactory *_meshAssetFactory;
        ShaderAssetFactory *_shaderAssetFactory;

        std::unordered_map<std::string, Entry> _assets;

        std::optional<std::jthread> _loadingThread;
        std::mutex _loadingMutex;
        std::queue<std::string> _loadingQueue;

        void tryLoadAsset(const std::string &asset);

        [[nodiscard]] std::shared_ptr<Asset> loadShaderByPath(std::filesystem::path &&path);
        [[nodiscard]] std::shared_ptr<Asset> loadMeshByPath(std::filesystem::path &&path);
        [[nodiscard]] std::shared_ptr<Asset> loadImageByPath(std::filesystem::path &&path);
    };
}

#include "AssetManager.inl"

#endif // PENROSE_ASSETS_ASSET_MANAGER_HPP
