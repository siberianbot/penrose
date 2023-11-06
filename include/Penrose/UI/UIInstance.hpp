#ifndef PENROSE_UI_UI_INSTANCE_HPP
#define PENROSE_UI_UI_INSTANCE_HPP

#include <map>
#include <memory>
#include <optional>
#include <tuple>

#include <Penrose/Assets/UILayoutAsset.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/ValueBinding.hpp>
#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class UIInstance {
    public:

        UIInstance(std::shared_ptr<UILayoutAsset> &&layoutAsset,
                   std::unique_ptr<ObjectValueProxy> &&rootContext,
                   ValueBindings &&valueBindings)
                : _layoutAsset(std::forward<decltype(layoutAsset)>(layoutAsset)),
                  _rootContext(std::forward<decltype(rootContext)>(rootContext)),
                  _valueBindings(std::forward<decltype(valueBindings)>(valueBindings)) {
            //
        }

        [[nodiscard]] const Layout *getLayout() const { return this->_layoutAsset->getLayout(); }

        [[nodiscard]] const ObjectValueProxy *getRootContext() const { return this->_rootContext.get(); }

        [[nodiscard]] std::shared_ptr<Penrose::ValueProxy> getBinding(const ValueBindingKey &key) const {
            auto it = this->_valueBindings.find(key);

            if (it == this->_valueBindings.end()) {
                throw EngineError("No such binding");
            }

            return it->second;
        }

    private:
        std::shared_ptr<UILayoutAsset> _layoutAsset;
        std::unique_ptr<ObjectValueProxy> _rootContext;
        ValueBindings _valueBindings;
    };
}

#endif // PENROSE_UI_UI_INSTANCE_HPP
