#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Engine.hpp>
#include <Penrose/Rendering/RenderManager.hpp>

using namespace Penrose;

int main() {

    Engine engine;

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->addDir("data");

    auto renderManager = engine.resources().get<RenderManager>();
    renderManager->setExecutionInfo(RenderExecutionInfo {
        .renderers =
            {
                        RendererExecutionInfo("Default", Params().add("View", "Default")),
                        },
    });

    engine.run();
}
