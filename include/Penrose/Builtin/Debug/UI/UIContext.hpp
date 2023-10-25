#ifndef PENROSE_BUILTIN_DEBUG_UI_UI_CONTEXT_HPP
#define PENROSE_BUILTIN_DEBUG_UI_UI_CONTEXT_HPP

#include <map>
#include <memory>
#include <string>

#include <Penrose/Api.hpp>
#include <Penrose/Resources/Resource.hpp>

#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

namespace Penrose {

    class PENROSE_API UIContext : public Resource<UIContext, ResourceGroup::Rendering> {
    public:
        ~UIContext() override = default;

        void setRoot(const std::string &name, std::shared_ptr<Widget> &&widget);
        void removeRoot(const std::string &name);

        [[nodiscard]] const std::map<std::string, std::shared_ptr<Widget>> &getRoots() const { return this->_roots; }

    private:
        std::map<std::string, std::shared_ptr<Widget>> _roots;
    };
}

#endif // PENROSE_BUILTIN_DEBUG_UI_UI_CONTEXT_HPP
