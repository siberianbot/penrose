#include "RenderGraphExecutorProvider.hpp"

#include <map>
#include <string>

#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderSubgraphFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"

#include "src/Builtin/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    RenderGraphExecutorProvider::RenderGraphExecutorProvider(ResourceSet *resources)
            : _resources(resources),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()),
              _renderSubgraphFactory(resources->get<RenderSubgraphFactory>()) {
        //
    }

    RenderGraphExecutor *RenderGraphExecutorProvider::createFor(const RenderGraphInfo &graph) {
        std::map<std::string, VkRenderTarget *> targets;
        for (const auto &[name, target]: graph.getTargets()) {
            targets.emplace(name, nullptr);
        }

        std::map<std::string, RenderOperator *> operatorMap;
        auto operators = this->_resources->getAll<RenderOperator>();

        for (const auto &op: operators) {
            operatorMap.emplace(op->getName(), op);
        }

        std::map<std::string, RenderGraphExecutor::SubgraphEntry> subgraphs;
        for (const auto &[name, subgraph]: graph.getSubgraphs()) {
            auto renderSubgraph = dynamic_cast<VkRenderSubgraph *>(
                    this->_renderSubgraphFactory->makeRenderSubgraph(subgraph));

            subgraphs.emplace(name, RenderGraphExecutor::SubgraphEntry{
                    .renderSubgraph = renderSubgraph,
                    .framebuffer = nullptr
            });
        }

        return new RenderGraphExecutor(this->_deviceContext, this->_presentContext, operatorMap,
                                       this->_resources->get<VkRenderTargetFactory>(),
                                       graph, targets, subgraphs);
    }
}
