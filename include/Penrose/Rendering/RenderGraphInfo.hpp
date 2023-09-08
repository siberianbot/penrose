#ifndef PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP

#include <map>
#include <set>
#include <string>

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>
#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderGraphInfo {
    public:
        explicit RenderGraphInfo(std::map<std::string, RenderTargetInfo> &&targets = {},
                                 std::map<std::string, RenderSubgraphInfo> &&subgraphs = {},
                                 std::map<std::string, std::set<std::string>> &&dependencies = {})
                : _targets(targets),
                  _subgraphs(subgraphs),
                  _dependencies(dependencies) {
            //
        }

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

        [[nodiscard]] RenderGraphInfo &addDependency(const std::string &subgraphName,
                                                     const std::string &dependencySubgraphName) {
            auto it = this->_dependencies.find(subgraphName);

            if (it != this->_dependencies.end()) {
                it->second.insert(dependencySubgraphName);
            } else {
                this->_dependencies.emplace(subgraphName, std::set<std::string>{dependencySubgraphName});
            }

            return *this;
        }

        [[nodiscard]] const std::map<std::string, RenderTargetInfo> &getTargets() const { return this->_targets; }

        [[nodiscard]] const std::map<std::string, RenderSubgraphInfo> &getSubgraphs() const { return this->_subgraphs; }

        [[nodiscard]] const std::map<std::string, std::set<std::string>> &getDependencies() const {
            return this->_dependencies;
        }

        [[nodiscard]] bool operator==(const RenderGraphInfo &rhs) const = default;

    private:
        std::map<std::string, RenderTargetInfo> _targets;
        std::map<std::string, RenderSubgraphInfo> _subgraphs;
        std::map<std::string, std::set<std::string>> _dependencies;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_INFO_HPP
