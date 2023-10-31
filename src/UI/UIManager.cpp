#include <Penrose/UI/UIManager.hpp>

#include <variant>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/Widgets/Button.hpp>
#include <Penrose/UI/Widgets/Checkbox.hpp>
#include <Penrose/UI/Widgets/Input.hpp>
#include <Penrose/UI/Widgets/Label.hpp>
#include <Penrose/UI/Widgets/Window.hpp>

namespace Penrose {

    UIManager::UIManager(ResourceSet *resources)
            : _assetManager(resources->get<AssetManager>()) {

        this->_widgetBinders[WidgetType::Window] = [](WidgetBinderContext &context) {
            auto window = dynamic_cast<const Window *>(context.currentWidget);

            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &window->getTitle());
            auto nestedContext = addBindOnlyBinding(context.bindings, context.currentWidget, context.currentContext,
                                                    &window->getContext());

            for (const auto &child: window->getChildren()) {
                context.widgets.emplace(child.get(), nestedContext);
            }
        };

        this->_widgetBinders[WidgetType::Button] = [](WidgetBinderContext &context) {
            auto button = dynamic_cast<const Button *>(context.currentWidget);

            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &button->getTitle());
            addBindOnlyBinding(context.bindings, context.currentWidget, context.currentContext,
                               &button->getAction());
        };

        this->_widgetBinders[WidgetType::Checkbox] = [](WidgetBinderContext &context) {
            auto checkbox = dynamic_cast<const Checkbox *>(context.currentWidget);

            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &checkbox->getText());
            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &checkbox->getChecked());
        };

        this->_widgetBinders[WidgetType::Input] = [](WidgetBinderContext &context) {
            auto input = dynamic_cast<const Input *>(context.currentWidget);

            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &input->getText());
        };

        this->_widgetBinders[WidgetType::Label] = [](WidgetBinderContext &context) {
            auto label = dynamic_cast<const Label *>(context.currentWidget);

            addStrongTypedBinding(context.bindings, context.currentWidget, context.currentContext,
                                  &label->getText());
        };
    }

    void UIManager::destroy() {
        this->_instances.clear();
    }

    UIInstance *UIManager::createUI(std::string &&name,
                                    std::string_view &&layoutAsset,
                                    std::unique_ptr<ObjectValueProxy> &&rootContext) {

        if (this->_instances.contains(name)) {
            throw EngineError("UI {} already exists", name);
        }

        ValueBindings bindings;

        auto asset = this->_assetManager->getAsset<UILayoutAsset>(std::forward<decltype(layoutAsset)>(layoutAsset));

        std::queue<ContextualizedWidget> widgets;
        widgets.emplace(asset->getLayout()->getRoot(), rootContext);

        while (!widgets.empty()) {
            auto [widget, context] = widgets.front();
            widgets.pop();

            addStrongTypedBinding(bindings, widget, context, &widget->getVisible());
            addStrongTypedBinding(bindings, widget, context, &widget->getEnabled());

            auto binderIt = this->_widgetBinders.find(widget->getType());

            if (binderIt == this->_widgetBinders.end()) {
                throw EngineError("Widget type is not supported for binding");
            }

            auto binderContext = WidgetBinderContext{
                    .bindings = bindings,
                    .widgets = widgets,
                    .currentWidget = widget,
                    .currentContext = context
            };

            binderIt->second(binderContext);
        }

        auto [it, _] = this->_instances.emplace(name, std::make_unique<UIInstance>(
                std::forward<decltype(asset)>(asset),
                std::forward<decltype(rootContext)>(rootContext),
                std::forward<decltype(bindings)>(bindings)));

        return it->second.get();
    }

    std::optional<UIInstance *> UIManager::tryGetUI(std::string &&name) const {
        auto it = this->_instances.find(name);

        if (it == this->_instances.end()) {
            return std::nullopt;
        }

        return it->second.get();
    }

    void UIManager::destroyUI(std::string &&name) {
        this->_instances.erase(name);
    }
}
