#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    PipelineShaderStage &PipelineShaderStage::setType(PipelineShaderStageType type) {
        this->_type = type;

        return *this;
    }

    PipelineShaderStage &PipelineShaderStage::setShaderAssetName(const std::string &shaderAssetName) {
        this->_shaderAssetName = shaderAssetName;

        return *this;
    }

    PipelineBindingAttribute &PipelineBindingAttribute::setFormat(PipelineBindingAttributeFormat format) {
        this->_format = format;

        return *this;
    }

    PipelineBindingAttribute &PipelineBindingAttribute::setOffset(std::uint32_t offset) {
        this->_offset = offset;

        return *this;
    }

    PipelineBinding &PipelineBinding::setInputRate(PipelineBindingInputRate inputRate) {
        this->_inputRate = inputRate;

        return *this;
    }

    PipelineBinding &PipelineBinding::setStride(std::uint32_t stride) {
        this->_stride = stride;

        return *this;
    }

    PipelineBinding &PipelineBinding::addAttribute(PipelineBindingAttribute &&attribute) {
        this->_attributes.push_back(attribute);

        return *this;
    }

    PipelineInfo &PipelineInfo::addStage(PipelineShaderStage &&stage) {
        this->_stages.push_back(stage);

        return *this;
    }

    PipelineInfo &PipelineInfo::addBinding(PipelineBinding &&binding) {
        this->_bindings.push_back(binding);

        return *this;
    }
}
