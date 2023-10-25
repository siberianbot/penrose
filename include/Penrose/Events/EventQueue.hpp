#ifndef PENROSE_EVENTS_EVENT_QUEUE_HPP
#define PENROSE_EVENTS_EVENT_QUEUE_HPP

#include <atomic>
#include <array>
#include <cstdint>
#include <functional>
#include <list>
#include <type_traits>
#include <variant>
#include <vector>

#include <Penrose/Api.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>
#include <Penrose/Utils/TemplateUtils.hpp>

namespace Penrose {

    template<typename E>
    concept IsEvent = std::is_base_of_v<Event<E>, E>;

    template<typename ...Events> requires All<IsEvent<Events>...>
    class PENROSE_API EventQueue : public Resource<EventQueue<Events...>, ResourceGroup::Events>,
                                   public Initializable,
                                   public Updatable {
    public:
        ~EventQueue() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override {
            this->_handlers.clear();

            for (auto &eventQueue: this->_eventQueues) {
                eventQueue.clear();
            }
        }

        void update(float) override {
            this->swap();

            for (const auto &event: this->back()) {
                for (const auto &handler: this->_handlers) {
                    handler(&event);
                }
            }

            this->back().clear();
        }

        template<typename E>
        requires Any<std::is_same_v<E, Events>...> && IsEvent<E> && std::is_default_constructible_v<E>
        void push() {
            auto event = E::create();

            this->current().emplace_back(event);
        }

        template<typename E, typename ...Args>
        requires Any<std::is_same_v<E, Events>...> && IsEvent<E> && std::is_constructible_v<E, Args...>
        void push(Args &&...args) {
            auto event = E::create(std::forward<decltype(args)>(args)...);

            this->current().emplace_back(event);
        }

        template<typename E, typename H>
        requires Any<std::is_same_v<E, Events>...> && IsEvent<E> && std::is_invocable_v<H, const E *>
        void addHandler(H handler) {
            this->_handlers.emplace_back([handler](const EventVariant *event) {
                auto targetEvent = std::get_if<E>(event);

                if (targetEvent == nullptr) {
                    return;
                }

                handler(targetEvent);
            });
        }

    private:
        constexpr static const std::size_t QUEUE_COUNT = 2;

        using EventVariant = std::variant<Events...>;
        using Queue = std::vector<EventVariant>;
        using Handler = std::function<void(const EventVariant *)>;

        std::list<Handler> _handlers;
        std::array<Queue, QUEUE_COUNT> _eventQueues;
        std::atomic_size_t _currentEventQueueIdx = 0;

        [[nodiscard]] Queue &current() {
            return this->_eventQueues.at(this->_currentEventQueueIdx % QUEUE_COUNT);
        }

        [[nodiscard]] Queue &back() {
            return this->_eventQueues.at((this->_currentEventQueueIdx - 1) % QUEUE_COUNT);
        }

        void swap() {
            this->_currentEventQueueIdx++;
        }
    };
}

#endif // PENROSE_EVENTS_EVENT_QUEUE_HPP
