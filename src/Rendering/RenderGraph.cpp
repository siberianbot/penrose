#include "RenderGraph.hpp"

namespace Penrose {

    RenderGraph makeDefaultRenderGraph() {
        return RenderGraph{
                .targets = {
                        RenderTarget{
                                .source = RenderTargetSource::Swapchain
                        }
                },
                .subgraphs = {
                        RenderSubgraph{
                                .dependsOn = {},
                                .attachments = {
                                        RenderAttachment{
                                                .targetIdx = 0,
                                                .clear = {.color = {0, 0, 0, 1}},
                                                .loadOp = RenderAttachmentLoadOp::Clear,
                                                .storeOp = RenderAttachmentStoreOp::Store,
                                                .initialLayout = RenderAttachmentLayout::Undefined,
                                                .finalLayout = RenderAttachmentLayout::Present
                                        }
                                },
                                .passes = {
                                        RenderPass{
                                                .dependsOn = {},
                                                .colorAttachments = {0}
                                        }
                                }
                        }
                }
        };
    }
}
