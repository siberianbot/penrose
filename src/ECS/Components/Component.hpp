#ifndef PENROSE_ECS_COMPONENTS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_COMPONENT_HPP

#include "src/ECS/ECSBase.hpp"

namespace Penrose {

    class Component {
    public:
        virtual ~Component() = default;
    };
}

#endif // PENROSE_ECS_COMPONENTS_COMPONENT_HPP
