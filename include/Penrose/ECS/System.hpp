#ifndef PENROSE_ECS_SYSTEM_HPP
#define PENROSE_ECS_SYSTEM_HPP

#include <string>

namespace Penrose {

    /**
     * \brief ECS system interface
     * \details Systems are implementing internal game logic. System manager runs all systems in separate thread to keep
     * engine running. Systems may not implement all its methods. Default implementation of init/destroy/update does
     * nothing. System name is required.
     */
    class System {
    public:
        virtual ~System() = default;

        /**
         * \brief Get system name
         * \return System name
         */
        [[nodiscard]] virtual std::string getName() const = 0;

        /**
         * \brief Initialize system
         */
        virtual void init() {
            // nothing to do
        }

        /**
         * \brief Deinitialize system
         */
        virtual void destroy() {
            // nothing to do
        }

        /**
         * \brief Update system state
         * \param delta Difference in milliseconds between system updates
         */
        virtual void update(float delta) {
            // nothing to do
        }
    };
}

#endif // PENROSE_ECS_SYSTEM_HPP
