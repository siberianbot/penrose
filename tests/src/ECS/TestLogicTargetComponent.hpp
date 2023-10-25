#ifndef PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP
#define PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

using namespace Penrose;

class TestLogicTargetComponent : public Component<TestLogicTargetComponent> {
public:
    ~TestLogicTargetComponent() override = default;
};

using TestLogicTargetComponentFactory = GenericComponentFactory<TestLogicTargetComponent>;

#endif // PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP
