#include "AssetIndex.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "AssetIndex";

    AssetIndex::AssetIndex(ResourceSet *resources)
        : _log(resources->get<Log>()) {
        //
    }

    void AssetIndex::add(std::string_view &&asset, std::filesystem::path &&path) {
        const auto assetStr = std::string(asset);

        // ReSharper disable once CppUseStructuredBinding
        auto &entry = this->_entries[assetStr];

        entry.path = path;
        entry.state.store(State::Unloaded);
        entry.instance.store(nullptr);
    }

    void AssetIndex::remove(std::string_view &&asset) {
        this->_entries.erase(std::string(asset));
    }

    void AssetIndex::removeAll() {
        this->_entries.clear();
    }

    void AssetIndex::markUnloaded(std::string_view &&asset) noexcept {
        const auto it = this->_entries.find(std::string(asset));

        if (it == this->_entries.end()) {
            this->_log->writeError(TAG, "Asset {} not found", asset);

            return;
        }

        it->second.state.store(State::Unloaded);
        it->second.instance.store(nullptr);
    }

    void AssetIndex::markLoading(std::string_view &&asset) noexcept {
        const auto it = this->_entries.find(std::string(asset));

        if (it == this->_entries.end()) {
            this->_log->writeError(TAG, "Asset {} not found", asset);

            return;
        }

        it->second.state.store(State::Loading);
    }

    void AssetIndex::markLoaded(std::string_view &&asset, std::shared_ptr<Asset> &&instance) noexcept {
        const auto it = this->_entries.find(std::string(asset));

        if (it == this->_entries.end()) {
            this->_log->writeError(TAG, "Asset {} not found", asset);

            return;
        }

        it->second.instance.store(std::forward<decltype(instance)>(instance));
        it->second.state.store(State::Loaded);
    }

    void AssetIndex::markFailed(std::string_view &&asset) noexcept {
        const auto it = this->_entries.find(std::string(asset));

        if (it == this->_entries.end()) {
            this->_log->writeError(TAG, "Asset {} not found", asset);

            return;
        }

        it->second.state.store(State::Failed);
    }

    std::optional<AssetIndex::Entry> AssetIndex::tryGet(std::string_view &&asset) const noexcept {
        const auto it = this->_entries.find(std::string(asset));

        if (it == this->_entries.end()) {
            return std::nullopt;
        }

        return Entry {
            .path = it->second.path,
            .state = it->second.state.load(),
            .instance = it->second.instance.load(),
        };
    }
}
