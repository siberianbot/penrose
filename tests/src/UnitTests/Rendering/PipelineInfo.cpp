#include <catch2/catch.hpp>

#include <Penrose/Rendering/PipelineInfo.hpp>

using namespace Penrose;

TEST_CASE("PipelineInfo_HashEquality", "[engine-unit-test]") {
    auto a = PipelineInfo()
            .addStage(PipelineShaderStage(Penrose::PipelineShaderStageType::Vertex, "asset"))
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Vertex, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            )
            .setLayout(
                    PipelineLayout()
                            .addBinding(PipelineLayoutBinding(
                                    Penrose::PipelineShaderStageType::Vertex,
                                    Penrose::PipelineLayoutBindingType::Sampler,
                                    1
                            ))
                            .addConstant(PipelineLayoutConstant(
                                    Penrose::PipelineShaderStageType::Fragment,
                                    0, 42
                            ))
            );

    auto b = PipelineInfo()
            .addStage(PipelineShaderStage(Penrose::PipelineShaderStageType::Vertex, "asset"))
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Vertex, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            )
            .setLayout(
                    PipelineLayout()
                            .addBinding(PipelineLayoutBinding(
                                    Penrose::PipelineShaderStageType::Vertex,
                                    Penrose::PipelineLayoutBindingType::Sampler,
                                    1
                            ))
                            .addConstant(PipelineLayoutConstant(
                                    Penrose::PipelineShaderStageType::Fragment,
                                    0, 42
                            ))
            );

    CHECK(std::hash<PipelineInfo>{}(a) == std::hash<PipelineInfo>{}(b));
}

TEST_CASE("PipelineInfo_HashInequality", "[engine-unit-test]") {
    auto a = PipelineInfo()
            .addStage(PipelineShaderStage(Penrose::PipelineShaderStageType::Vertex, "asset"))
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Vertex, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            )
            .setLayout(
                    PipelineLayout()
                            .addBinding(PipelineLayoutBinding(
                                    Penrose::PipelineShaderStageType::Vertex,
                                    Penrose::PipelineLayoutBindingType::Sampler,
                                    1
                            ))
                            .addConstant(PipelineLayoutConstant(
                                    Penrose::PipelineShaderStageType::Fragment,
                                    0, 42
                            ))
            );

    auto b = PipelineInfo()
            .addStage(PipelineShaderStage(Penrose::PipelineShaderStageType::Fragment, "assetb"))
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Instance, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            )
            .setLayout(
                    PipelineLayout()
                            .addBinding(PipelineLayoutBinding(
                                    Penrose::PipelineShaderStageType::Fragment,
                                    Penrose::PipelineLayoutBindingType::Sampler,
                                    1
                            ))
                            .addConstant(PipelineLayoutConstant(
                                    Penrose::PipelineShaderStageType::Vertex,
                                    0, 42
                            ))
            );

    CHECK_FALSE(std::hash<PipelineInfo>{}(a) == std::hash<PipelineInfo>{}(b));
}

TEST_CASE("PipelineInfo_HashInequalityWithDifferentOrder", "[engine-unit-test]") {
    auto a = PipelineInfo()
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Vertex, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            )
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Instance, 43)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec2, 12))
            );

    auto b = PipelineInfo()
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Instance, 43)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec2, 12))
            )
            .addBinding(
                    PipelineBinding(Penrose::PipelineBindingInputRate::Vertex, 42)
                            .addAttribute(PipelineBindingAttribute(Penrose::PipelineBindingAttributeFormat::Vec3, 13))
            );

    CHECK_FALSE(std::hash<PipelineInfo>{}(a) == std::hash<PipelineInfo>{}(b));
}
