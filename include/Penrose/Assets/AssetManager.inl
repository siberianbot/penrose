#ifndef PENROSE_ASSETS_ASSET_MANAGER_INL
#define PENROSE_ASSETS_ASSET_MANAGER_INL

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template<IsAsset T>
    constexpr std::optional<std::shared_ptr<T>> toConcreteAsset(const std::shared_ptr<Asset> &ptr) {
        auto castedPtr = std::dynamic_pointer_cast<T>(ptr);

        if (castedPtr == nullptr) {
            return std::optional<std::shared_ptr<T>>{};
        }

        return std::optional<std::shared_ptr<T>>(castedPtr);
    }

    template<IsAsset T>
    std::optional<std::shared_ptr<T>> AssetManager::tryGetAsset(const std::string &asset, bool wait) const {
        auto maybeAsset = this->tryGetAsset(asset, wait);

        if (!maybeAsset.has_value()) {
            return std::nullopt;
        }

        return orElseThrow(
                flatMap(maybeAsset, toConcreteAsset<T>),
                EngineError(fmt::format("Asset {} miscast: not a {}", asset, typeid(T).name()))
        );
    }

    template<IsAsset T>
    std::shared_ptr<T> AssetManager::getAsset(const std::string &asset) const {
        return orElseThrow(this->tryGetAsset<T>(asset, true), EngineError(fmt::format("Asset {} not loaded", asset)));
    }
}

#endif // PENROSE_ASSETS_ASSET_MANAGER_INL
