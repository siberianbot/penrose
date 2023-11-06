#ifndef PENROSE_UI_UI_MANAGER_HPP
#define PENROSE_UI_UI_MANAGER_HPP

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <tuple>

#include <Penrose/Api.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIInstance.hpp>

namespace Penrose {

    class PENROSE_API UIManager : public Resource<UIManager, ResourceGroup::UI>,
                                  public Initializable {
    public:
        explicit UIManager(ResourceSet *resources);
        ~UIManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        UIInstance *createUI(std::string &&name,
                             std::string_view &&layoutAsset,
                             std::unique_ptr<ObjectValueProxy> &&rootContext);

        [[nodiscard]] std::optional<UIInstance *> tryGetUI(std::string &&name) const;

        void destroyUI(std::string &&name);

    private:
        using ContextualizedWidget = std::tuple<const Widget *, const ObjectValueProxy *>;

        struct WidgetBinderContext {
            ValueBindings &bindings;
            std::queue<ContextualizedWidget> &widgets;
            const Widget *currentWidget;
            const ObjectValueProxy *currentContext;
        };

        using WidgetBinder = std::function<void(WidgetBinderContext &)>;

        ResourceProxy<AssetManager> _assetManager;

        std::map<WidgetType, WidgetBinder> _widgetBinders;
        std::map<std::string, std::unique_ptr<UIInstance>> _instances;
    };
}

#endif // PENROSE_UI_UI_MANAGER_HPP
