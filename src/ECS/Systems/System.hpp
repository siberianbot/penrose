#ifndef PENROSE_ECS_SYSTEMS_SYSTEM_HPP
#define PENROSE_ECS_SYSTEMS_SYSTEM_HPP

#include <type_traits>

namespace Penrose {

    class System {
    public:
        virtual ~System() = default;

        virtual void init() { /* nothing to do */ }

        virtual void update() { /* nothing to do */ }

        virtual void destroy() { /* nothing to do */ }
    };
}

#endif // PENROSE_ECS_SYSTEMS_SYSTEM_HPP
