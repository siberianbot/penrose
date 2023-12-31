#ifndef PENROSE_UI_LAYOUT_FACTORY_HPP
#define PENROSE_UI_LAYOUT_FACTORY_HPP

#include <functional>
#include <map>
#include <string>

#include <libxml++/nodes/element.h>

#include <Penrose/Resources/Resource.hpp>
#include <Penrose/UI/Layout.hpp>

namespace Penrose {

    class LayoutFactory final: public Resource<LayoutFactory> {
    public:
        LayoutFactory();
        ~LayoutFactory() override = default;

        [[nodiscard]] Layout *makeLayout(std::vector<unsigned char> &&content);

    private:
        using WidgetFactory = std::function<Widget *(const xmlpp::Element *)>;

        std::map<std::string, WidgetFactory> _factories;

        [[nodiscard]] Widget *makeWidget(const xmlpp::Element *element);
    };
}

#endif // PENROSE_UI_LAYOUT_FACTORY_HPP
