#ifndef PENROSE_ECS_ECS_ENTRY_HPP
#define PENROSE_ECS_ECS_ENTRY_HPP

#include <memory>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    struct ECSEntry {
        Entity entity;
        std::string componentName;
        std::shared_ptr<Component> component;
    };
}

#endif // PENROSE_ECS_ECS_ENTRY_HPP
