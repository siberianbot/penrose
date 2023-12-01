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

    void UIManager::createContext(std::string_view &&name) {
        const auto nameStr = std::string(std::forward<decltype(name)>(name));

        if (this->_contexts.contains(nameStr)) {
            throw EngineError("UI context {} already exists", nameStr);
        }

        this->_contexts.emplace(nameStr, std::make_unique<UIContext>());
    }

    void UIManager::destroyContext(std::string_view &&name) {
        const auto nameStr = std::string(name);

        this->_contexts.erase(nameStr);
    }

    void UIManager::addLayoutToContext(
        std::string_view &&name, std::string_view &&layout, std::shared_ptr<ObjectValue> &&valueContext
    ) {
        const auto nameStr = std::string(name);
        const auto it = this->_contexts.find(nameStr);

        if (it == this->_contexts.end()) {
            throw EngineError("UI context {} does not exists", nameStr);
        }

        auto loadedLayout = this->_assetManager->getAsset<UILayoutAsset>(std::forward<decltype(layout)>(layout))
                                ->getLayout()
                                .lock();

        it->second->pushLayout(std::move(loadedLayout), std::forward<decltype(valueContext)>(valueContext));
    }

    std::optional<UIContext *> UIManager::tryGetContext(std::string_view &&name) const {
        const auto nameStr = std::string(name);

        if (const auto it = this->_contexts.find(nameStr); it != this->_contexts.end()) {
            return it->second.get();
        }

        return std::nullopt;
    }
}
