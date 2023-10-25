#include <Penrose/ECS/SystemManager.hpp>

namespace Penrose {

    SystemManager::SystemManager(ResourceSet *resources)
            : _systems(resources->get<System>()) {
        //
    }

    void SystemManager::update(float delta) {
        for (const auto &system: this->_systems) {
            system->update(delta);
        }
    }
}
