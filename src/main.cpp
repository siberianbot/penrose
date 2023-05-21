#include "src/Core/Engine.hpp"
#include "src/Rendering/RenderContext.hpp"

int main() {
    Penrose::Engine engine;

    auto graph = Penrose::makeDefaultRenderGraph();
    engine.resources().get<Penrose::RenderContext>()->setRenderGraph(graph);

    engine.run();

    return 0;
}
