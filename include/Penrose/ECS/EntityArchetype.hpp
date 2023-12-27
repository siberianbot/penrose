#ifndef PENROSE_ECS_ENTITY_ARCHETYPE_HPP
#define PENROSE_ECS_ENTITY_ARCHETYPE_HPP

#include <memory>
#include <string>
#include <vector>

#include <Penrose/Common/Params.hpp>
#include <Penrose/ECS/Component.hpp>

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
         * \brief Construct components for new entity
         * \param params Construction parameters
         * \return Collection of components
         */
        [[nodiscard]] virtual std::vector<std::shared_ptr<ComponentPtr>> construct(const Params &params) = 0;
    };
}

#endif // PENROSE_ECS_ENTITY_ARCHETYPE_HPP
