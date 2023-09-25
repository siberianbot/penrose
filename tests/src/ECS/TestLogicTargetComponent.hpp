#ifndef PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP
#define PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

using namespace Penrose;

class TestLogicTargetComponent : public Component {
public:
    ~TestLogicTargetComponent() override = default;

    [[nodiscard]] std::string getName() const override { return std::string(name()); }

    [[nodiscard]] constexpr static std::string_view name() { return "TestLogicTarget"; }
};

class TestLogicTargetComponentFactory : public Resource, public GenericComponentFactory<TestLogicTargetComponent> {
public:
    ~TestLogicTargetComponentFactory() override = default;
};

#endif // PENROSE_TESTS_ECS_TEST_LOGIC_TARGET_COMPONENT_HPP
