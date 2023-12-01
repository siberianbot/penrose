#ifndef PENROSE_ASSETS_ASSET_MANAGER_HPP
#define PENROSE_ASSETS_ASSET_MANAGER_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include <type_traits>

#include <Penrose/Assets/Asset.hpp>

namespace Penrose {

    /**
     * \brief Asset management interface
     * \details This interface provides methods for working with assets in Penrose.
     */
    class PENROSE_API AssetManager {
    public:
        virtual ~AssetManager() = default;

        /**
         * \brief Add directory to internal asset index
         * \param rootDir root directory, where index file is located
         */
        virtual void addDir(std::filesystem::path &&rootDir) = 0;

        /**
         * \brief Try to add directory to internal asset index
         * \param rootDir root directory, where index file is located
         */
        virtual void tryAddDir(std::filesystem::path &&rootDir) = 0;

        /**
         * \brief Enqueue loading of asset
         * \param asset name of asset
         */
        virtual void load(std::string_view &&asset) = 0;

        /**
         * \brief Unload asset immediately
         * \param asset name of asset
         */
        virtual void unload(std::string_view &&asset) = 0;

        /**
         * \brief Get asset
         * \details This method implicitly loads and awaits asset. EngineError is thrown on any kind of error.
         * \param asset name of asset
         * \return shared pointer to asset instance
         */
        [[nodiscard]] virtual std::shared_ptr<Asset> getAsset(std::string_view &&asset) = 0;

        /**
         * \brief Try to get asset
         * \details This method only checks for asset existance and returns only loaded assets. Any error is logged and
         * nothing is returned.
         * \param asset name of asset
         * \return shared pointer to asset instance or nothing
         */
        [[nodiscard]] virtual std::optional<std::shared_ptr<Asset>> tryGetAsset(std::string_view &&asset) = 0;

        /**
         * \brief Get asset of concrete type T
         * \details This method implicitly loads and awaits asset. EngineError is thrown on any kind of error.
         * \tparam T type of asset
         * \param asset name of asset
         * \return shared pointer to asset instance
         */
        template <typename T>
        requires std::is_base_of_v<Asset, T>
        [[nodiscard]] std::shared_ptr<T> getAsset(std::string_view &&asset) {
            return std::dynamic_pointer_cast<T>(this->getAsset(std::forward<decltype(asset)>(asset)));
        }

        /**
         * \brief Try to get asset of concrete type T
         * \details This method only checks for asset existance and returns only loaded assets. Any error is logged and
         * nothing is returned.
         * \tparam T type of asset
         * \param asset name of asset
         * \return shared pointer to asset instance or nothing
         */
        template <typename T>
        requires std::is_base_of_v<Asset, T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetAsset(std::string_view &&asset) {
            const auto maybePtr = this->tryGetAsset(std::forward<decltype(asset)>(asset));

            if (!maybePtr.has_value()) {
                return std::nullopt;
            }

            return std::dynamic_pointer_cast<T>(*maybePtr);
        }
    };
}

#endif // PENROSE_ASSETS_ASSET_MANAGER_HPP
