#ifndef PENROSE_ECS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENT_HPP

#include <typeindex>
#include <typeinfo>

namespace Penrose {

    /**
     * \brief Component type
     */
    using ComponentType = std::type_index;

    /**
     * \brief Pointer type of Component<>
     */
    struct PENROSE_API ComponentPtr {
        virtual ~ComponentPtr() = default;

        /**
         * \brief Get type of component
         * \return Type of component
         */
        [[nodiscard]] virtual ComponentType getType() const = 0;
    };

    /**
     * \brief Base type of entity' component
     * \details Components gives entities some traits, like transform, mesh, etc. Components are data oriented.
     * \tparam Self Type of Component<> inheritor
     */
    template <typename Self>
    struct PENROSE_API Component: ComponentPtr {
        Component(const Component &) = default;
        Component(Component &&) = default;
        Component &operator=(const Component &) = default;
        Component &operator=(Component &&) = default;

        ~Component() override = default;

        /**
         * \brief Get type of component
         * \return Type of component
         */
        [[nodiscard]] static ComponentType type() { return typeid(Self); }

        //! \copydoc ComponentPtr::getType
        [[nodiscard]] ComponentType getType() const final { return type(); }
    };
}

#endif // PENROSE_ECS_COMPONENT_HPP
