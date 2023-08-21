#ifndef PENROSE_RENDERING_RENDER_GRAPH_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_HPP

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>
#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderGraph {
    public:
        RenderGraph &setTarget(const std::string &name, const RenderTargetInfo &target);
        void removeTarget(const std::string &name);

        RenderGraph &setSubgraph(const std::string &name, const RenderSubgraphInfo &subgraph);
        void removeSubgraph(const std::string &name);

        [[nodiscard]] const std::map<std::string, RenderTargetInfo> &getTargets() const { return this->_targets; }

        [[nodiscard]] const std::map<std::string, RenderSubgraphInfo> &getSubgraphs() const { return this->_subgraphs; }

        static RenderGraph makeDefault();

        [[nodiscard]] bool operator==(const RenderGraph &rhs) const = default;

    private:
        std::map<std::string, RenderTargetInfo> _targets;
        std::map<std::string, RenderSubgraphInfo> _subgraphs;
    };

    [[nodiscard]] std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs);
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_HPP
