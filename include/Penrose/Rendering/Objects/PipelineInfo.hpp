#ifndef PENROSE_RENDERING_OBJECTS_PIPELINE_INFO_HPP
#define PENROSE_RENDERING_OBJECTS_PIPELINE_INFO_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace Penrose {

    /**
     * \brief Pipeline shader stage type
     */
    enum class PipelineShaderStage {
        Vertex,
        Fragment
    };

    /**
     * \brief Pipeline binding input rate
     */
    enum class PipelineInputRate {
        Vertex,
        Instance
    };

    /**
     * \brief Pipeline binding attribute format
     */
    enum class PipelineInputAttributeFormat {
        UInt,
        Vec2,
        Vec3,
        Mat4
    };

    /**
     * \brief Type of pipeline object binding
     */
    enum class PipelineObjectType {
        Sampler,
        InputAttachment,
        UniformBuffer
    };

    /**
     * \brief Pipeline shader stage definition
     */
    struct PENROSE_API PipelineShader {

        /**
         * \brief Type of shader stage
         */
        PipelineShaderStage stage;

        /**
         * \brief Name of shader asset
         */
        std::string shaderAssetName;

        friend auto operator<=>(const PipelineShader &, const PipelineShader &) = default;
    };

    /**
     * \brief Pipeline input attribute definition
     */
    struct PENROSE_API PipelineInputAttribute {

        /**
         * \brief Format of attribute
         */
        PipelineInputAttributeFormat format;

        /**
         * \brief Attribute offset
         */
        std::uint32_t offset;

        friend auto operator<=>(const PipelineInputAttribute &, const PipelineInputAttribute &) = default;
    };

    /**
     * \brief Pipeline input definition
     */
    struct PENROSE_API PipelineInput {

        /**
         * \brief Input rate
         */
        PipelineInputRate rate;

        /**
         * \brief Input stride
         */
        std::uint32_t stride;

        /**
         * \brief Input attributes
         */
        std::vector<PipelineInputAttribute> attributes;

        friend auto operator<=>(const PipelineInput &, const PipelineInput &) = default;
    };

    /**
     * \brief Pipeline constant definition
     */
    struct PENROSE_API PipelineConstant {

        /**
         * \brief Target shader stage
         */
        PipelineShaderStage stage;

        /**
         * \brief Offset
         */
        std::uint32_t offset;

        /**
         * \brief Size
         */
        std::uint32_t size;

        friend auto operator<=>(const PipelineConstant &, const PipelineConstant &) = default;
    };

    /**
     * \brief Pipeline object binding definition
     */
    struct PENROSE_API PipelineObject {

        /**
         * \brief Target shader stage
         */
        PipelineShaderStage stage;

        /**
         * \brief Type
         */
        PipelineObjectType type;

        /**
         * \brief Count
         */
        std::uint32_t count;

        friend auto operator<=>(const PipelineObject &, const PipelineObject &) = default;
    };

    /**
     * \brief Pipeline definition
     */
    struct PENROSE_API PipelineInfo {

        /**
         * \brief Pipeline name
         */
        std::string name;

        /**
         * \brief List of applied shaders
         */
        std::vector<PipelineShader> shaders;

        /**
         * \brief List of data inputs
         */
        std::vector<PipelineInput> inputs;

        /**
         * \brief List of constants
         */
        std::vector<PipelineConstant> constants;

        /**
         * \brief List of objects
         */
        std::vector<PipelineObject> objects;

        friend auto operator<=>(const PipelineInfo &, const PipelineInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_PIPELINE_INFO_HPP
