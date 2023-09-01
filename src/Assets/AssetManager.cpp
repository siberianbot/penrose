#include <Penrose/Assets/AssetManager.hpp>

#include <fmt/format.h>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr static const std::string_view ASSET_MANAGER_TAG = "AssetManager";

    AssetManager::AssetManager(ResourceSet *resources)
            : _assetDictionary(resources->getLazy<AssetDictionary>()),
              _assetLoader(resources->getLazy<AssetLoader>()),
              _log(resources->getLazy<Log>()) {
        //
    }

    void AssetManager::destroy() {
        this->_assets.clear();

        while (!this->_loadingQueue.empty()) {
            this->_loadingQueue.pop();
        }
    }

    void AssetManager::run() {
        this->_log->writeInfo(ASSET_MANAGER_TAG, "Initializing background loading thread");

        this->_loadingThread = std::jthread([this](const std::stop_token &stopToken) {
            while (!stopToken.stop_requested()) {
                auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

                while (!this->_loadingQueue.empty()) {
                    auto asset = this->_loadingQueue.front();
                    this->_loadingQueue.pop();

                    this->tryLoadAsset(asset);
                }
            }
        });
    }

    void AssetManager::stop() {
        if (this->_loadingThread.has_value()) {
            auto &thread = this->_loadingThread.value();

            thread.request_stop();

            if (thread.joinable()) {
                thread.join();
            }

            this->_loadingThread = std::nullopt;
        }
    }

    void AssetManager::enqueue(const std::string &asset) {
        auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

        auto entry = AssetManager::Entry{
                .state = LoadingState::Pending,
                .ptr = nullptr
        };

        this->_assets.emplace(asset, entry);
        this->_loadingQueue.push(asset);
    }

    bool AssetManager::isLoaded(const std::string &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return false;
        }

        return it->second.state == LoadingState::Loaded;
    }

    void AssetManager::unload(const std::string &asset) {
        this->_assets.erase(asset);

        this->_log->writeInfo(ASSET_MANAGER_TAG, fmt::format("Unloaded asset {}", asset));
    }

    void AssetManager::tryLoadAsset(const std::string &asset) {
        if (this->isLoaded(asset)) {
            return;
        }

        this->_log->writeInfo(ASSET_MANAGER_TAG, fmt::format("Loading asset {}", asset));

        auto maybePath = this->_assetDictionary->tryGetPath(asset);
        if (!maybePath.has_value()) {
            this->_log->writeError(ASSET_MANAGER_TAG, fmt::format("No asset {} available in dictionary", asset));
            return;
        }

        const auto &it = this->_assets.find(asset);
        if (it == this->_assets.end()) {
            this->_log->writeDebug(ASSET_MANAGER_TAG,
                                   fmt::format("Asset {} was queued for loading, but it didn't have an entry", asset));
            return;
        }

        try {
            it->second.ptr = std::shared_ptr<Asset>(this->_assetLoader->load(*maybePath));
            it->second.state = LoadingState::Loaded;
        } catch (const std::exception &error) {
            it->second.state = LoadingState::Failed;

            this->_log->writeError(ASSET_MANAGER_TAG, fmt::format("Failed to load asset {}: {}", asset, error.what()));
            return;
        }

        this->_log->writeInfo(ASSET_MANAGER_TAG, fmt::format("Loaded asset {}", asset));
    }

    template<>
    std::optional<std::shared_ptr<Asset>> AssetManager::tryGetAsset<Asset>(const std::string &asset, bool wait) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return std::nullopt;
        }

        if (it->second.state == LoadingState::Pending && wait) {
            while (it->second.state == LoadingState::Pending) {
                // wait
            }
        }

        if (it->second.state != LoadingState::Loaded) {
            return std::nullopt;
        }

        return it->second.ptr;
    }
}
