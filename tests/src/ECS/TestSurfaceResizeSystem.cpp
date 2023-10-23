#include "TestSurfaceResizeSystem.hpp"

#include <cmath>

#include <Penrose/Resources/ResourceSet.hpp>

TestSwapchainResizeSystem::TestSwapchainResizeSystem(ResourceSet *resources)
        : _surfaceManager(resources->get<SurfaceManager>()) {
    //
}

void TestSwapchainResizeSystem::init() {
    this->_passed = 0;
}

void TestSwapchainResizeSystem::update(float delta) {
    this->_passed += delta;

    std::uint32_t w = 1000 + static_cast<std::uint32_t>(500 * sin(this->_passed));
    std::uint32_t h = 1000 + static_cast<std::uint32_t>(500 * cos(this->_passed));

    this->_surfaceManager->getSurface()->setSize({w, h});
}
