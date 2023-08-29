#include "RenderGraphExecutorProvider.hpp"

#include <map>
#include <string>

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
            : _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()),
              _renderSubgraphFactory(resources->get<RenderSubgraphFactory>()),
              _renderOperators(resources->getAllLazy<RenderOperator>()),
              _renderTargetFactory(resources->getLazy<RenderTargetFactory>()) {
        //
    }

    RenderGraphExecutor *RenderGraphExecutorProvider::createFor(const RenderGraphInfo &graph) {
        std::map<std::string, VkRenderTarget *> targets;
        for (const auto &[name, target]: graph.getTargets()) {
            targets.emplace(name, nullptr);
        }

        std::map<std::string, RenderOperator *> operatorMap;

        for (auto &op: this->_renderOperators) {
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
                                       dynamic_cast<VkRenderTargetFactory *>(this->_renderTargetFactory.get()),
                                       graph, targets, subgraphs);
    }
}
