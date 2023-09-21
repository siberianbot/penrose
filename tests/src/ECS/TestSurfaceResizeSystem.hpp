#ifndef PENROSE_TESTS_ECS_TEST_SWAPCHAIN_RESIZE_SYSTEM_HPP
#define PENROSE_TESTS_ECS_TEST_SWAPCHAIN_RESIZE_SYSTEM_HPP

#include <Penrose/ECS/System.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

namespace Penrose {
    class ResourceSet;
}

class TestSwapchainResizeSystem : public Resource, public Initializable, public System {
public:
    explicit TestSwapchainResizeSystem(ResourceSet *resources);
    ~TestSwapchainResizeSystem() override = default;

    void init() override;

    void update(float delta) override;

    void destroy() override { /* nothing to do */ }

    [[nodiscard]] std::string getName() const override { return "TestSwapchainResizeSystem"; }

private:
    Lazy<SurfaceManager> _surfaceManager;

    double _passed = 0;
};


#endif // PENROSE_TESTS_ECS_TEST_SWAPCHAIN_RESIZE_SYSTEM_HPP
