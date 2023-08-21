#ifndef PENROSE_RENDERING_RENDER_SUBGRAPH_INFO_HPP
#define PENROSE_RENDERING_RENDER_SUBGRAPH_INFO_HPP

#include <optional>
#include <string>
#include <vector>

#include <Penrose/Common/Size.hpp>
#include <Penrose/Rendering/RenderAttachmentInfo.hpp>
#include <Penrose/Rendering/RenderSubgraphPassInfo.hpp>

namespace Penrose {

    class RenderSubgraphInfo {
    public:
        explicit RenderSubgraphInfo(std::vector<std::string> &&dependsOn = {},
                                    std::vector<RenderAttachmentInfo> &&attachments = {},
                                    std::vector<RenderSubgraphPassInfo> &&passes = {},
                                    std::optional<Size> &&renderArea = std::nullopt)
                : _dependsOn(dependsOn),
                  _attachments(attachments),
                  _passes(passes),
                  _renderArea(renderArea) {
            //
        }

        [[nodiscard]] RenderSubgraphInfo &setDependsOn(const std::vector<std::string> &dependsOn) {
            this->_dependsOn = dependsOn;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setDependsOn(std::vector<std::string> &&dependsOn) {
            this->_dependsOn = dependsOn;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setAttachments(const std::vector<RenderAttachmentInfo> &attachments) {
            this->_attachments = attachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setAttachments(std::vector<RenderAttachmentInfo> &&attachments) {
            this->_attachments = attachments;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setPasses(const std::vector<RenderSubgraphPassInfo> &passes) {
            this->_passes = passes;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setPasses(std::vector<RenderSubgraphPassInfo> &&passes) {
            this->_passes = passes;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setRenderArea(const std::optional<Size> &renderArea) {
            this->_renderArea = renderArea;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &setRenderArea(std::optional<Size> &&renderArea) {
            this->_renderArea = renderArea;

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addDependency(const std::string &dependency) {
            this->_dependsOn.push_back(dependency);

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addDependency(std::string &&dependency) {
            this->_dependsOn.push_back(dependency);

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addAttachment(const RenderAttachmentInfo &attachment) {
            this->_attachments.push_back(attachment);

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addAttachment(RenderAttachmentInfo &&attachment) {
            this->_attachments.push_back(attachment);

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addPass(const RenderSubgraphPassInfo &pass) {
            this->_passes.push_back(pass);

            return *this;
        }

        [[nodiscard]] RenderSubgraphInfo &addPass(RenderSubgraphPassInfo &&pass) {
            this->_passes.push_back(pass);

            return *this;
        }

        [[nodiscard]] const std::vector<std::string> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<RenderAttachmentInfo> &getAttachments() const { return this->_attachments; }

        [[nodiscard]] const std::vector<RenderSubgraphPassInfo> &getPasses() const { return this->_passes; }

        [[nodiscard]] const std::optional<Size> &getRenderArea() const { return this->_renderArea; }

        [[nodiscard]] bool operator==(const RenderSubgraphInfo &rhs) const = default;

    private:
        std::vector<std::string> _dependsOn;
        std::vector<RenderAttachmentInfo> _attachments;
        std::vector<RenderSubgraphPassInfo> _passes;
        std::optional<Size> _renderArea;
    };

}

#endif // PENROSE_RENDERING_RENDER_SUBGRAPH_INFO_HPP
