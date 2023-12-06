#ifndef PENROSE_TESTS_ECS_TEST_COUNTDOWN_SYSTEM_HPP
#define PENROSE_TESTS_ECS_TEST_COUNTDOWN_SYSTEM_HPP

#include <optional>
#include <thread>

#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EngineEvents.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

using namespace Penrose;

class TestCountdownSystem : public Resource<TestCountdownSystem>,
                            public Initializable,
                            public System {
public:
    constexpr static const float DEFAULT_TEST_TIMEOUT = 3.0f;
    constexpr static const float DEFAULT_FAILURE_TIMEOUT = 3.0f;

    explicit TestCountdownSystem(const ResourceSet *resources);
    ~TestCountdownSystem() override;

    void init() override;

    void update(float delta) override;

    void destroy() override { /* nothing to do */ }

    [[nodiscard]] std::string getName() const override { return "TestCountdownSystem"; }

    void setTestTimeout(float timeout) { this->_testTimeout = timeout; }

    void setFailureTimeout(float timeout) { this->_failureTimeout = timeout; }

private:
    ResourceProxy<EngineEventQueue> _eventQueue;

    float _passed = 0;
    float _testTimeout = DEFAULT_TEST_TIMEOUT;
    float _failureTimeout = DEFAULT_FAILURE_TIMEOUT;

    std::optional<std::jthread> _timeoutThread;
};

#endif // PENROSE_TESTS_ECS_TEST_COUNTDOWN_SYSTEM_HPP
