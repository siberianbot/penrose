#ifndef PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP
#define PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP

#include <memory>
#include <string>
#include <vector>

namespace Penrose {

    enum class WidgetType {
        Window,
        Label,
    };

    class Widget {
    public:
        virtual ~Widget() = default;

        [[nodiscard]] virtual WidgetType getType() const = 0;
    };

    class WidgetRoot {
    public:
        virtual ~WidgetRoot() = default;

        [[nodiscard]] virtual const std::vector<std::shared_ptr<Widget>> &getChilds() const = 0;
        [[nodiscard]] virtual std::vector<std::shared_ptr<Widget>> &getChilds() = 0;
    };

    class Window : public Widget, public WidgetRoot {
    public:
        explicit Window(std::string &&title, std::initializer_list<std::shared_ptr<Widget>> &&childs = {})
                : _title(title),
                  _childs(childs) {
            //
        }

        ~Window() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Window; }

        [[nodiscard]] const std::string &getTitle() const { return this->_title; }

        void setTitle(const std::string &title) { this->_title = title; }

        [[nodiscard]] const std::vector<std::shared_ptr<Widget>> &getChilds() const override { return this->_childs; }

        [[nodiscard]] std::vector<std::shared_ptr<Widget>> &getChilds() override { return this->_childs; }

    private:
        std::string _title;
        std::vector<std::shared_ptr<Widget>> _childs;
    };

    class Label : public Widget {
    public:
        explicit Label(std::string &&text)
                : _text(text) {
            //
        }

        ~Label() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Label; }

        [[nodiscard]] const std::string &getText() const { return this->_text; }

        void setText(const std::string &text) { this->_text = text; }

    private:
        std::string _text;
    };
}

#endif // PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP
