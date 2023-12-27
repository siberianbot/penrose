#ifndef PENROSE_ECS_SYSTEM_MANAGER_IMPL_HPP
#define PENROSE_ECS_SYSTEM_MANAGER_IMPL_HPP

#include <chrono>
#include <map>
#include <semaphore>
#include <string>

#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/SystemManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Common/JobQueue.hpp"

namespace Penrose {

    class SystemManagerImpl final: public Resource<SystemManagerImpl>,
                                   public SystemManager,
                                   public Initializable {
    public:
        explicit SystemManagerImpl(const ResourceSet *resources);
        ~SystemManagerImpl() override = default;

        void init() override;
        void destroy() override;

        void addSystem(std::type_index &&type, SystemParams &&params) override;

        void startSystem(std::string_view &&system) override;
        void stopSystem(std::string_view &&system) override;
        void pauseSystem(std::string_view &&system) override;
        void resumeSystem(std::string_view &&system) override;

        SystemState getSystemState(std::string_view &&system) override;

    private:
        struct Entry {
            System *instance;
            SystemParams params;
            SystemState state;
            SystemState failedState;
            bool initialized;
            std::optional<std::chrono::high_resolution_clock::time_point> lastUpdate;
        };

        const ResourceSet *_resources;
        ResourceProxy<Log> _log;

        JobQueue _jobQueue;
        std::binary_semaphore _semaphore;

        std::map<std::string, Entry> _systems;

        [[nodiscard]] Entry *getSystemEntry(std::string_view &&system);
        void changeState(std::string_view &&system, SystemState targetState, std::set<SystemState> &&requiredStates);

        void update();
        void handle(Entry &entry);
    };
}

#endif // PENROSE_ECS_SYSTEM_MANAGER_IMPL_HPP
