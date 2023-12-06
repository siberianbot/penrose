#ifndef PENROSE_ASSETS_ASSET_LOADING_JOB_QUEUE_HPP
#define PENROSE_ASSETS_ASSET_LOADING_JOB_QUEUE_HPP

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/AssetIndex.hpp"
#include "src/Assets/AssetLoadingProxy.hpp"
#include "src/Common/JobQueue.hpp"

namespace Penrose {

    class AssetLoadingJobQueue final: public Resource<AssetLoadingJobQueue>,
                                      public Initializable {
    public:
        explicit AssetLoadingJobQueue(const ResourceSet *resources);
        ~AssetLoadingJobQueue() override = default;

        void init() override;
        void destroy() override;

        void enqueue(std::string_view &&asset);

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<AssetIndex> _assetIndex;
        ResourceProxy<AssetLoadingProxy> _assetLoadingProxy;

        JobQueue _jobQueue;
    };
}

#endif // PENROSE_ASSETS_ASSET_LOADING_JOB_QUEUE_HPP
