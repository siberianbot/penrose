#include "SystemManagerImpl.hpp"

#include <set>

#include <Penrose/Common/EngineError.hpp>

#include "src/Utils/SyncUtils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "SystemManagerImpl";

    SystemManagerImpl::SystemManagerImpl(const ResourceSet *resources)
        : _resources(resources),
          _log(resources->get<Log>()),
          _semaphore(1) {
        //
    }

    void SystemManagerImpl::init() {
        this->_log->writeInfo(TAG, "Initializing system manager");

        this->_jobQueue.enqueue([this] { this->update(); }, {.remove = false});
        this->_jobQueue.start();
    }

    void SystemManagerImpl::destroy() {
        this->_log->writeInfo(TAG, "Deinitializing system manager");

        {
            const auto guard = SemaphoreGuard(this->_semaphore);
            for (auto &entry: this->_systems | std::views::values) {
                entry.state = SystemState::Stopping;
            }
        }

        this->_jobQueue.stop();
        this->_jobQueue.clear();
    }

    void SystemManagerImpl::addSystem(std::type_index &&type, SystemParams &&params) {
        if (this->_jobQueue.running()) {
            throw EngineError("System manager is running");
        }

        const auto system = this->_resources->resolveOne<System>(type);

        if (this->_systems.contains(system->getName())) {
            throw EngineError("System {} already added into system manager", system->getName());
        }

        this->_systems.emplace(
            system->getName(),
            Entry {
                .instance = system,
                .params = params,
                .state = SystemState::Stopped,
                .failedState = SystemState::Stopped,
                .initialized = false,
                .lastUpdate = std::nullopt
            }
        );
    }

    void SystemManagerImpl::startSystem(std::string_view &&system) {
        this->changeState(
            std::forward<decltype(system)>(system), SystemState::Initializing,
            {SystemState::Stopped, SystemState::Failed}
        );
    }

    void SystemManagerImpl::stopSystem(std::string_view &&system) {
        this->changeState(
            std::forward<decltype(system)>(system), SystemState::Stopping, {SystemState::Running, SystemState::Paused}
        );
    }

    void SystemManagerImpl::pauseSystem(std::string_view &&system) {
        this->changeState(std::forward<decltype(system)>(system), SystemState::Paused, {SystemState::Running});
    }

    void SystemManagerImpl::resumeSystem(std::string_view &&system) {
        this->changeState(std::forward<decltype(system)>(system), SystemState::Running, {SystemState::Paused});
    }

    SystemState SystemManagerImpl::getSystemState(std::string_view &&system) {
        return this->getSystemEntry(std::forward<decltype(system)>(system))->state;
    }

    SystemManagerImpl::Entry *SystemManagerImpl::getSystemEntry(std::string_view &&system) {
        const auto it = this->_systems.find(std::string(system));

        if (it == this->_systems.end()) {
            throw EngineError("System {} not found", system);
        }

        return &it->second;
    }

    void SystemManagerImpl::changeState(
        std::string_view &&system, const SystemState targetState, std::set<SystemState> &&requiredStates
    ) {
        const auto entry = this->getSystemEntry(std::forward<decltype(system)>(system));
        const auto guard = SemaphoreGuard(this->_semaphore);

        if (!requiredStates.contains(entry->state)) {
            return;
        }

        entry->state = targetState;
    }

    void SystemManagerImpl::update() {
        for (auto &[name, entry]: this->_systems) {
            try {
                this->handle(entry);
            } catch (const std::exception &error) {
                entry.failedState = entry.state;
                entry.state = SystemState::Failed;

                this->_log->writeError(TAG, "System {} failure: {}", name, error.what());
            }
        }
    }

    void SystemManagerImpl::handle(Entry &entry) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        switch (entry.state) {
            case SystemState::Stopped:
                {
                    if (entry.params.autostart == SystemAutoStartParam::None
                        || entry.params.autostart == SystemAutoStartParam::Once && entry.initialized) {
                        return;
                    }

                    entry.state = SystemState::Initializing;
                }
                break;

            case SystemState::Initializing:
                {
                    entry.initialized = true;
                    entry.instance->init();
                    entry.state = SystemState::Running;
                }
                break;

            case SystemState::Running:
                {
                    const float delta = entry.lastUpdate.has_value()
                                            ? std::chrono::duration<float>(
                                                  std::chrono::high_resolution_clock::now() - *entry.lastUpdate
                                              )
                                                  .count()
                                            : 0;

                    entry.instance->update(delta);
                    entry.lastUpdate = std::chrono::high_resolution_clock::now();
                }
                break;

            case SystemState::Paused:
                {
                    entry.lastUpdate = std::nullopt;
                }
                break;

            case SystemState::Stopping:
                {
                    entry.instance->destroy();
                    entry.state = SystemState::Stopped;
                }
                break;

            case SystemState::Failed:
                {
                    if (!entry.params.recover) {
                        return;
                    }

                    entry.state = entry.failedState;
                }
                break;

            default:
                throw EngineError::notImplemented();
        }
    }
}
