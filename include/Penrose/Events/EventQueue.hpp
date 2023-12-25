#ifndef PENROSE_EVENTS_EVENT_QUEUE_HPP
#define PENROSE_EVENTS_EVENT_QUEUE_HPP

#include <array>
#include <atomic>
#include <functional>
#include <list>
#include <type_traits>
#include <variant>
#include <vector>

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>
#include <Penrose/Utils/TemplateUtils.hpp>

namespace Penrose {

    /**
     * \brief Event queue
     * \details Event queue allows to push event and dispatch them to corresponding handlers. All added handlers are
     * removed on queue deinitialization. Event queue is double buffered.
     * \tparam Events Type of events that are processable by this queue
     */
    template <typename... Events>
    class PENROSE_API EventQueue final: public Resource<EventQueue<Events...>>,
                                        public Initializable,
                                        public Updatable {
    public:
        ~EventQueue() override = default;

        //! \copydoc Initializable::init
        void init() override {
            // nothing to do
        }

        //! \copydoc Initializable::destroy
        void destroy() override {
            this->_handlers.clear();

            for (auto &eventQueue: this->_eventQueues) {
                eventQueue.clear();
            }
        }

        //! \copydoc Updatable::update
        void update(float) override {
            this->swap();

            for (const auto &event: this->back()) {
                for (const auto &handler: this->_handlers) {
                    handler(&event);
                }
            }

            this->back().clear();
        }

        /**
         * \brief Construct and push event
         * \tparam E Type of event
         */
        template <typename E>
        requires Any<std::is_same_v<E, Events>...> && std::is_default_constructible_v<E>
        void push() {
            this->current().emplace_back(E());
        }

        /**
         * \brief Push event
         * \tparam E Type of event
         * \param event Instance of new event
         */
        template <typename E>
        requires Any<std::is_same_v<E, Events>...>
        void push(E &&event) {
            this->current().emplace_back(std::forward<decltype(event)>(event));
        }

        /**
         * \brief Add event handler function
         * \tparam E Type of event
         * \param handler Event handler function
         */
        template <typename E>
        requires Any<std::is_same_v<E, Events>...>
        void addHandler(std::function<void(const E *)> &&handler) {
            this->_handlers.emplace_back([handler](const EventVariant *event) {
                const auto targetEvent = std::get_if<E>(event);

                if (targetEvent == nullptr) {
                    return;
                }

                handler(targetEvent);
            });
        }

    private:
        static constexpr std::size_t QUEUE_COUNT = 2;

        using EventVariant = std::variant<Events...>;
        using Queue = std::vector<EventVariant>;
        using Handler = std::function<void(const EventVariant *)>;

        std::list<Handler> _handlers;
        std::array<Queue, QUEUE_COUNT> _eventQueues;
        std::atomic_size_t _currentEventQueueIdx = 0;

        [[nodiscard]] Queue &current() { return this->_eventQueues.at(this->_currentEventQueueIdx % QUEUE_COUNT); }

        [[nodiscard]] Queue &back() { return this->_eventQueues.at((this->_currentEventQueueIdx - 1) % QUEUE_COUNT); }

        void swap() { ++this->_currentEventQueueIdx; }
    };
}

#endif // PENROSE_EVENTS_EVENT_QUEUE_HPP
