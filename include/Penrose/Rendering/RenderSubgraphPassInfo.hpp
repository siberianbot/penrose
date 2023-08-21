#ifndef PENROSE_RENDERING_RENDER_SUBGRAPH_PASS_INFO_HPP
#define PENROSE_RENDERING_RENDER_SUBGRAPH_PASS_INFO_HPP

#include <cstdint>
#include <optional>
#include <vector>

#include <Penrose/Rendering/RenderOperatorInfo.hpp>

namespace Penrose {

    class RenderSubgraphPassInfo {
    public:
        explicit RenderSubgraphPassInfo(std::vector<std::uint32_t> &&dependsOn = {},
                                        std::vector<std::uint32_t> &&inputAttachments = {},
                                        std::vector<std::uint32_t> &&colorAttachments = {},
                                        std::optional<std::uint32_t> &&depthStencilAttachment = std::nullopt,
                                        std::optional<RenderOperatorInfo> &&operatorInfo = std::nullopt)
                : _dependsOn(dependsOn),
                  _inputAttachments(inputAttachments),
                  _colorAttachments(colorAttachments),
                  _depthStencilAttachment(depthStencilAttachment),
                  _operatorInfo(operatorInfo) {
            //
        }

        [[nodiscard]] RenderSubgraphPassInfo &setDependsOn(const std::vector<std::uint32_t> &dependsOn) {
            this->_dependsOn = dependsOn;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setDependsOn(std::vector<std::uint32_t> &&dependsOn) {
            this->_dependsOn = dependsOn;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setInputAttachments(const std::vector<std::uint32_t> &inputAttachments) {
            this->_inputAttachments = inputAttachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setInputAttachments(std::vector<std::uint32_t> &&inputAttachments) {
            this->_inputAttachments = inputAttachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setColorAttachments(const std::vector<std::uint32_t> &colorAttachments) {
            this->_colorAttachments = colorAttachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setColorAttachments(std::vector<std::uint32_t> &&colorAttachments) {
            this->_colorAttachments = colorAttachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setDepthStencilAttachment(
                const std::optional<std::uint32_t> &depthStencilAttachment) {
            this->_depthStencilAttachment = depthStencilAttachment;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setDepthStencilAttachment(
                std::optional<std::uint32_t> &&depthStencilAttachment) {
            this->_depthStencilAttachment = depthStencilAttachment;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setOperatorInfo(const std::optional<RenderOperatorInfo> &operatorInfo) {
            this->_operatorInfo = operatorInfo;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &setOperatorInfo(std::optional<RenderOperatorInfo> &&operatorInfo) {
            this->_operatorInfo = operatorInfo;

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &addDependencyIdx(std::uint32_t dependencyIdx) {
            this->_dependsOn.push_back(dependencyIdx);

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &addInputAttachmentIdx(std::uint32_t attachmentIdx) {
            this->_inputAttachments.push_back(attachmentIdx);

            return *this;
        }

        [[nodiscard]] RenderSubgraphPassInfo &addColorAttachmentIdx(std::uint32_t attachmentIdx) {
            this->_colorAttachments.push_back(attachmentIdx);

            return *this;
        }

        [[nodiscard]] const std::vector<std::uint32_t> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<std::uint32_t> &getInputAttachments() const { return this->_inputAttachments; }

        [[nodiscard]] const std::vector<std::uint32_t> &getColorAttachments() const { return this->_colorAttachments; }

        [[nodiscard]] const std::optional<std::uint32_t> &getDepthStencilAttachment() const {
            return this->_depthStencilAttachment;
        }

        [[nodiscard]] const std::optional<RenderOperatorInfo> &getOperator() const { return this->_operatorInfo; }

        [[nodiscard]] bool operator==(const RenderSubgraphPassInfo &rhs) const = default;

    private:
        std::vector<std::uint32_t> _dependsOn;
        std::vector<std::uint32_t> _inputAttachments;
        std::vector<std::uint32_t> _colorAttachments;
        std::optional<std::uint32_t> _depthStencilAttachment;
        std::optional<RenderOperatorInfo> _operatorInfo;
    };
}

#endif // PENROSE_RENDERING_RENDER_SUBGRAPH_PASS_INFO_HPP
