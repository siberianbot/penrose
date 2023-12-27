#ifndef PENROSE_ECS_ENTITY_ARCHETYPE_HPP
#define PENROSE_ECS_ENTITY_ARCHETYPE_HPP

#include <string>

#include <Penrose/Common/Params.hpp>
#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    /**
     * \brief Entity archetype interface
     * \details Entity archetypes contains logic of creating entities with pre-determined set of components. Treat them
     * as factories of entities with ready-made components.
     */
    class PENROSE_API EntityArchetype {
    public:
        virtual ~EntityArchetype() = default;

        /**
         * \brief Get name of entity archetype
         * \return Name of entity archetype
         */
        [[nodiscard]] virtual std::string getName() const = 0;

        /**
         * \brief Construct entity
         * \param params Construction parameters
         * \return Entity
         */
        [[nodiscard]] virtual Entity construct(const Params &params) = 0;
    };
}

#endif // PENROSE_ECS_ENTITY_ARCHETYPE_HPP
