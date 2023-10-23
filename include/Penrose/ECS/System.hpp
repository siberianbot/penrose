#ifndef PENROSE_ECS_SYSTEM_HPP
#define PENROSE_ECS_SYSTEM_HPP

#include <string>

namespace Penrose {

    class System {
    public:
        virtual ~System() = default;

        virtual void update(float) = 0; // TODO: remove and implement Updatable

        [[nodiscard]] virtual std::string getName() const = 0;
    };
}

#endif // PENROSE_ECS_SYSTEM_HPP
