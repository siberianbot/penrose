#include <Penrose/UI/UIManager.hpp>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    UIManager::UIManager(ResourceSet *resources)
        : _assetManager(resources->get<AssetManager>()) {
        //
    }

    void UIManager::destroy() {
        this->_contexts.clear();
    }

    UIContext *UIManager::createContext(
        std::string_view &&name,
        std::string_view &&layout,
        std::shared_ptr<ObjectValue> &&rootContext
    ) {
        const auto nameStr = std::string(std::forward<decltype(name)>(name));

        if (this->_contexts.contains(nameStr)) {
            throw EngineError("UI context {} already exists", nameStr);
        }

        auto context = new UIContext(
            this->_assetManager->getAsset<UILayoutAsset>(std::forward<decltype(layout)>(layout)),
            std::forward<decltype(rootContext)>(rootContext)
        );

        this->_contexts.emplace(nameStr, context);

        return context;
    }

    void UIManager::destroyContext(std::string_view &&name) {
        const auto nameStr = std::string(name);

        this->_contexts.erase(nameStr);
    }

    std::optional<UIContext *> UIManager::tryGetContext(std::string_view &&name) const {
        const auto nameStr = std::string(name);

        if (const auto it = this->_contexts.find(nameStr); it != this->_contexts.end()) {
            return it->second.get();
        }

        return std::nullopt;
    }
}
