#include "AssetManagerImpl.hpp"

#include "src/Assets/Utils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "AssetManagerImpl";

    AssetManagerImpl::AssetManagerImpl(ResourceSet *resources)
        : _log(resources->get<Log>()),
          _assetIndex(resources->get<AssetIndex>()),
          _assetLoadingJobQueue(resources->get<AssetLoadingJobQueue>()) {
        //
    }

    void AssetManagerImpl::init() {
        /* nothing to do */
    }

    void AssetManagerImpl::destroy() {
        this->_assetIndex->removeAll();
    }

    void AssetManagerImpl::addDir(std::filesystem::path &&rootDir) {
        this->_log->writeInfo(TAG, "Adding directory {}", rootDir.string());

        IndexFile index = readIndexFile(std::filesystem::path(rootDir));

        std::vector<std::string> preload;

        for (const auto &[asset, entry]: index) {
            this->_assetIndex->add(asset, std::filesystem::path(entry.path));

            if (entry.preload) {
                preload.push_back(asset);
            }

            this->_log->writeDebug(TAG, "Added asset {} (from {})", asset, entry.path.string());
        }

        for (const auto &asset: preload) {
            this->_assetLoadingJobQueue->enqueue(asset);

            this->_log->writeDebug(TAG, "Enqueued asset {} for preloading", asset);
        }
    }

    void AssetManagerImpl::tryAddDir(std::filesystem::path &&rootDir) {
        try {
            this->addDir(std::filesystem::path(rootDir));
        } catch (const std::exception &error) {
            this->_log->writeError(TAG, "Failed to add directory {}: {}", rootDir.string(), error.what());
        }
    }

    void AssetManagerImpl::load(std::string_view &&asset) {
        this->_assetLoadingJobQueue->enqueue(std::forward<decltype(asset)>(asset));
    }

    void AssetManagerImpl::unload(std::string_view &&asset) {
        this->_assetIndex->markUnloaded(std::forward<decltype(asset)>(asset));
    }

    std::shared_ptr<Asset> AssetManagerImpl::getAsset(std::string_view &&asset) {
        std::optional<AssetIndex::Entry> maybeIndexEntry;

        do {
            maybeIndexEntry = this->_assetIndex->tryGet(std::string_view(asset));

            if (!maybeIndexEntry.has_value()) {
                throw EngineError("Asset {} not found", asset);
            }

            if (maybeIndexEntry->state == AssetIndex::State::Unloaded) {
                this->_assetLoadingJobQueue->enqueue(std::string_view(asset));
            }
        } while (maybeIndexEntry->state == AssetIndex::State::Loading);

        if (maybeIndexEntry->state == AssetIndex::State::Failed) {
            throw EngineError("Failed to load asset {}", asset);
        }

        if (maybeIndexEntry->instance.expired()) {
            throw EngineError("Asset {} expired", asset);
        }

        return maybeIndexEntry->instance.lock();
    }

    std::optional<std::shared_ptr<Asset>> AssetManagerImpl::tryGetAsset(std::string_view &&asset) {
        const auto maybeIndexEntry = this->_assetIndex->tryGet(std::string_view(asset));

        if (!maybeIndexEntry.has_value()) {
            this->_log->writeError(TAG, "Asset {} not found", asset);

            return std::nullopt;
        }

        if (maybeIndexEntry->state != AssetIndex::State::Loaded) {
            this->_log->writeWarning(TAG, "Asset {} not loaded", asset);

            return std::nullopt;
        }

        if (maybeIndexEntry->instance.expired()) {
            this->_log->writeError(TAG, "Asset {} expired", asset);

            return std::nullopt;
        }

        return maybeIndexEntry->instance.lock();
    }
}
