#include "RenderGraphExecutorProvider.hpp"

#include <map>
#include <string>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderOperatorFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"

#include "src/Builtin/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Rendering/VkRenderPass.hpp"
#include "src/Builtin/Rendering/VkRenderTarget.hpp"

namespace Penrose {

    RenderGraphExecutorProvider::RenderGraphExecutorProvider(ResourceSet *resources)
            : _resources(resources),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()) {
        //
    }

    RenderGraphExecutor *RenderGraphExecutorProvider::createFor(const RenderGraph &graph) {
        std::map<std::string, VkRenderTarget *> targets;
        for (const auto &[name, target]: graph.getTargets()) {
            targets.emplace(name, makeVkRenderTarget(this->_deviceContext,
                                                     this->_presentContext,
                                                     target));
        }

        std::map<std::string, RenderOperatorFactory *> factoryMap;
        auto factories = this->_resources->getAll<RenderOperatorFactory>();

        for (const auto &factory: factories) {
            factoryMap.emplace(factory->name(), factory);
        }

        std::map<std::string, RenderGraphExecutor::SubgraphEntry> subgraphs;
        for (const auto &[name, subgraph]: graph.getSubgraphs()) {
            auto renderPass = makeVkRenderPass(this->_deviceContext,
                                               this->_presentContext,
                                               subgraph);

            auto operators = std::vector<RenderOperator *>(subgraph.getPasses().size());
            for (std::uint32_t passIdx = 0; passIdx < subgraph.getPasses().size(); passIdx++) {
                auto pass = subgraph.getPasses().at(passIdx);

                if (!pass.getOperator().has_value()) {
                    operators[passIdx] = nullptr;
                    continue;
                }

                auto operatorName = pass.getOperator()->getName();
                auto factoryIt = factoryMap.find(operatorName);

                if (factoryIt == factoryMap.end()) {
                    throw EngineError(fmt::format("No factory for operator {}", operatorName));
                }

                auto createContext = RenderOperatorFactory::Context{
                        .params =factoryIt->second->defaults()
                                .merge(pass.getOperator()->getParams()),
                        .renderPass = renderPass->getRenderPass(),
                        .subpassIdx = passIdx
                };

                operators[passIdx] = factoryIt->second->create(createContext);
            }

            subgraphs.emplace(name, RenderGraphExecutor::SubgraphEntry{
                    .subgraph = subgraph,
                    .renderPass = renderPass,
                    .renderOperators = std::move(operators),
                    .framebuffer = nullptr
            });
        }

        return new RenderGraphExecutor(this->_deviceContext, this->_presentContext,
                                       graph, targets, subgraphs);
    }
}
