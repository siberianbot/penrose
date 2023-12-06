#include "AssetLoadingJobQueue.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "AssetLoadingJobQueue";

    AssetLoadingJobQueue::AssetLoadingJobQueue(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _assetIndex(resources->get<AssetIndex>()),
          _assetLoadingProxy(resources->get<AssetLoadingProxy>()) {
        //
    }

    void AssetLoadingJobQueue::init() {
        this->_jobQueue.start();
    }

    void AssetLoadingJobQueue::destroy() {
        this->_jobQueue.stop();
        this->_jobQueue.clear();
    }

    void AssetLoadingJobQueue::enqueue(std::string_view &&asset) {
        const auto maybeIndexEntry = this->_assetIndex->tryGet(std::string_view(asset));

        if (!maybeIndexEntry.has_value()) {
            this->_log->writeError(TAG, "Attempt to load unknown asset {}", asset);

            return;
        }

        if (maybeIndexEntry->state == AssetIndex::State::Loading
            || maybeIndexEntry->state == AssetIndex::State::Loaded) {
            return;
        }

        this->_assetIndex->markLoading(std::string_view(asset));

        this->_jobQueue.enqueue([this, asset = std::string(asset), path = (maybeIndexEntry->path)] {
            this->_log->writeDebug(TAG, "Loading asset {}", asset);

            try {
                Asset *instance = this->_assetLoadingProxy->fromPath(std::filesystem::path(path));

                this->_assetIndex->markLoaded(asset, std::shared_ptr<Asset>(instance));
                this->_log->writeDebug(TAG, "Loaded asset {}", asset);
            } catch (const std::exception &error) {
                this->_assetIndex->markFailed(asset);
                this->_log->writeError(TAG, "Failed to load asset {}: {}", asset, error.what());
            }
        });
    }
}
