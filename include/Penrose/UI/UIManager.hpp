#ifndef PENROSE_UI_UI_MANAGER_HPP
#define PENROSE_UI_UI_MANAGER_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIContext.hpp>
#include <Penrose/UI/Value.hpp>

namespace Penrose {

    class PENROSE_API UIManager final: public Resource<UIManager, ResourceGroup::UI>,
                                       public Initializable {
    public:
        explicit UIManager(ResourceSet *resources);
        ~UIManager() override = default;

        void init() override {
            // nothing to do
        }

        void destroy() override;

        UIContext *createContext(
            std::string_view &&name,
            std::string_view &&layout,
            std::shared_ptr<ObjectValue> &&rootContext
        );

        void destroyContext(std::string_view &&name);

        [[nodiscard]] std::optional<UIContext *> tryGetContext(std::string_view &&name) const;

    private:
        ResourceProxy<AssetManager> _assetManager;

        std::map<std::string, std::unique_ptr<UIContext>> _contexts;
    };
}

#endif // PENROSE_UI_UI_MANAGER_HPP
