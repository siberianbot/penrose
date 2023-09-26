#include <Penrose/Builtin/Debug/UI/UIContext.hpp>

namespace Penrose {

    void UIContext::setRoot(const std::string &name, std::shared_ptr<Widget> &&widget) {
        this->_roots.insert_or_assign(name, widget);
    }

    void UIContext::removeRoot(const std::string &name) {
        this->_roots.erase(name);
    }
}
