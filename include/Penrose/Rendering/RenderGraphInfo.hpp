#ifndef PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP

#include <map>
#include <string>

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>
#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderGraphInfo {
    public:
        [[nodiscard]] RenderGraphInfo &setTarget(const std::string &name, const RenderTargetInfo &target) {
            this->_targets.insert_or_assign(name, target);

            return *this;
        }

        [[nodiscard]] RenderGraphInfo &setTarget(const std::string &name, RenderTargetInfo &&target) {
            this->_targets.insert_or_assign(name, target);

            return *this;
        }

        void removeTarget(const std::string &name) {
            this->_targets.erase(name);
        }

        [[nodiscard]] RenderGraphInfo &setSubgraph(const std::string &name, const RenderSubgraphInfo &subgraph) {
            this->_subgraphs.insert_or_assign(name, subgraph);

            return *this;
        }

        [[nodiscard]] RenderGraphInfo &setSubgraph(const std::string &name, RenderSubgraphInfo &&subgraph) {
            this->_subgraphs.insert_or_assign(name, subgraph);

            return *this;
        }

        void removeSubgraph(const std::string &name) {
            this->_subgraphs.erase(name);
        }

        [[nodiscard]] const std::map<std::string, RenderTargetInfo> &getTargets() const { return this->_targets; }

        [[nodiscard]] const std::map<std::string, RenderSubgraphInfo> &getSubgraphs() const { return this->_subgraphs; }

        [[nodiscard]] bool operator==(const RenderGraphInfo &rhs) const = default;

    private:
        std::map<std::string, RenderTargetInfo> _targets;
        std::map<std::string, RenderSubgraphInfo> _subgraphs;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP
