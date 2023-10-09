#ifndef PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP
#define PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Penrose {

    enum class WidgetType {
        Window,
        Label,
        TextInput,
        Button,
    };

    class Widget {
    public:
        virtual ~Widget() = default;

        [[nodiscard]] virtual WidgetType getType() const = 0;

        [[nodiscard]] const bool &getVisible() const { return this->_visible; }

        void setVisible(bool visible) { this->_visible = visible; }

    private:
        bool _visible = true;
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

    class TextInput : public Widget {
    public:
        constexpr static const std::size_t DEFAULT_BUFFER_SIZE = 128;

        explicit TextInput(std::size_t size, std::optional<std::string> &&text = std::nullopt)
                : _size(size) {
            this->_text.resize(size);

            if (text.has_value()) {
                std::copy_n(text->begin(), std::min(size, text->size()), this->_text.begin());
            }
        }

        explicit TextInput(std::optional<std::string> &&text)
                : TextInput(text.has_value() ? text->size() : DEFAULT_BUFFER_SIZE,
                            std::forward<decltype(text)>(text)) {
            //
        }

        ~TextInput() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::TextInput; }

        [[nodiscard]] const std::size_t &getSize() const { return this->_size; }

        [[nodiscard]] const std::string &getText() const { return this->_text; }

        void grow() {
            this->_size = this->_size * 2;
            this->_text.resize(this->_size);
        }

    private:
        std::size_t _size;
        std::string _text;
    };

    class Button : public Widget {
    public:
        using Callback = std::function<void()>;

        explicit Button(std::string &&title, Callback &&callback)
                : _title(title),
                  _callback(callback) {
            //
        }

        ~Button() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Button; }

        [[nodiscard]] const std::string &getTitle() const { return this->_title; }

        void invoke() {
            this->_callback();
        }

    private:
        std::string _title;
        Callback _callback;
    };
}

#endif // PENROSE_BUILTIN_DEBUG_UI_WIDGETS_HPP
