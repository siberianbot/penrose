#include <csignal>
#include <iostream>
#include <string>

#include "src/Core/Engine.hpp"
#include "src/Rendering/RenderContext.hpp"
#include "src/Rendering/Operators/ImGuiDrawRenderOperator.hpp"

void printError(const std::exception &error, int level) {
    if (level > 1) {
        std::cerr << std::string(2 * level, ' ');
    }

    if (level != 0) {
        std::cerr << "-> ";
    }

    std::cerr << error.what() << std::endl;

    try {
        std::rethrow_if_nested(error);
    } catch (const std::exception &nested) {
        printError(nested, level + 1);
    }
}

int main() {
    std::set_terminate([]() {
        auto caught = std::current_exception();

        if (caught) {
            std::cerr << "Unhandled engine exception" << std::endl;

            try {
                std::rethrow_exception(caught);
            } catch (const std::exception &error) {
                printError(error, 0);
            }
        } else {
            std::cerr << "Abnormal termination" << std::endl;
        }

#if defined(SIGTRAP)
        std::raise(SIGTRAP);
#endif

        std::abort();
    });

    Penrose::Engine engine;

    auto graph = Penrose::makeDefaultRenderGraph();
    graph.subgraphs.at(0).passes.at(0).operatorName = Penrose::IMGUI_DRAW_RENDER_OPERATOR_NAME;

    auto renderContext = engine.resources().get<Penrose::RenderContext>();
    renderContext->addRenderOperatorProducer<Penrose::ImGuiDrawRenderOperatorProducer>(
            Penrose::IMGUI_DRAW_RENDER_OPERATOR_NAME);
    renderContext->setRenderGraph(graph);

    engine.run();

    return 0;
}
