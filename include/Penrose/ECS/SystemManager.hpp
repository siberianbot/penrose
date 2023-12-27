#ifndef PENROSE_ECS_SYSTEM_MANAGER_HPP
#define PENROSE_ECS_SYSTEM_MANAGER_HPP

#include <typeindex>
#include <typeinfo>

#include <Penrose/ECS/System.hpp>
#include <Penrose/ECS/SystemParams.hpp>
#include <Penrose/ECS/SystemState.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    /**
     * \brief System manager interface
     * \details Any systems within system manager can have different states. System manager is responsible for handling
     * this states.
     */
    class PENROSE_API SystemManager {
    public:
        virtual ~SystemManager() = default;

        /**
         * \brief Add system into system manager
         * \param type Type of system
         * \param params System parameters
         */
        virtual void addSystem(std::type_index &&type, SystemParams &&params) = 0;

        /**
         * \brief Add system into system manager
         * \tparam T Type of system
         * \param params System parameters
         */
        template <typename T>
        requires std::is_base_of_v<System, T>
        void addSystem(SystemParams &&params) {
            this->addSystem(typeid(T), std::forward<decltype(params)>(params));
        }

        /**
         * \brief Start system
         * \param system Name of system
         */
        virtual void startSystem(std::string_view &&system) = 0;

        /**
         * \brief Stop system
         * \param system Name of system
         */
        virtual void stopSystem(std::string_view &&system) = 0;

        /**
         * \brief Pause system updates
         * \param system Name of system
         */
        virtual void pauseSystem(std::string_view &&system) = 0;

        /**
         * \brief Resume system updates
         * \param system Name of system
         */
        virtual void resumeSystem(std::string_view &&system) = 0;

        /**
         * \brief Get current system state
         * \param system Name of system
         * \return Current system state
         */
        virtual SystemState getSystemState(std::string_view &&system) = 0;
    };
}

#endif // PENROSE_ECS_SYSTEM_MANAGER_HPP
