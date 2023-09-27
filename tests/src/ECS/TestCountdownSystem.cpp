#include "TestCountdownSystem.hpp"

#include <chrono>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Events/EngineEvent.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

TestCountdownSystem::TestCountdownSystem(ResourceSet *resources)
        : _eventQueue(resources->getLazy<EventQueue>()) {
    //
}

TestCountdownSystem::~TestCountdownSystem() {
    if (this->_timeoutThread.has_value() && this->_timeoutThread->request_stop() &&
        this->_timeoutThread->joinable()) {
        this->_timeoutThread->join();
    }
}

void TestCountdownSystem::init() {
    this->_passed = 0;
}

void TestCountdownSystem::update(float delta) {
    this->_passed += delta;

    if (this->_passed > this->_testTimeout) {
        auto data = EngineEventArgs{
                .type = EngineEventType::DestroyRequested
        };

        this->_eventQueue->pushEvent<EventType::EngineEvent>(data);

        if (!this->_timeoutThread.has_value()) {
            this->_timeoutThread = std::jthread([this](const std::stop_token &token) {
                auto startTime = std::chrono::high_resolution_clock::now();
                auto now = startTime;

                while (std::chrono::duration<float>(now - startTime).count() < this->_failureTimeout &&
                       !token.stop_requested()) {
                    now = std::chrono::high_resolution_clock::now();
                }

                if (token.stop_requested()) {
                    return;
                }

                throw EngineError("Timeout, test failed");
            });
        }
    }
}
