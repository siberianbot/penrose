#ifndef PENROSE_ECS_SYSTEM_MANAGER_HPP
#define PENROSE_ECS_SYSTEM_MANAGER_HPP

#include <Penrose/Api.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Resources/Updatable.hpp>

namespace Penrose {

    class PENROSE_API SystemManager : public Resource<SystemManager>,
                                      public Updatable {
    public:
        explicit SystemManager(ResourceSet *resources);
        ~SystemManager() override = default;

        void update(float delta) override;

    private:
        ResourceProxy<System> _systems;
    };
}

#endif // PENROSE_ECS_SYSTEM_MANAGER_HPP
