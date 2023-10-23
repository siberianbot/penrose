#include <Penrose/Rendering/RenderListBuilder.hpp>

#include <queue>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

namespace Penrose {

    RenderListBuilder::RenderListBuilder(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()),
              _sceneManager(resources->get<SceneManager>()),
              _drawableProviders(resources->get<DrawableProvider>()),
              _viewProviders(resources->get<ViewProvider>()) {
        //
    }

    void RenderListBuilder::init() {
        this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::ECSEvent, ECSEventArgs>(
                [this](const ECSEvent *event) {
                    auto lock = std::lock_guard<std::mutex>(this->_mutex);

                    switch (event->getArgs().type) {
                        case ECSEventType::ComponentCreated: {
                            this->handleComponentCreate(event->getArgs());
                            break;
                        }

                        case ECSEventType::ComponentDestroyed: {
                            this->handleComponentDestroy(event->getArgs());
                            break;
                        }

                        default:
                            /* nothing to do */
                            break;
                    }
                });
    }

    void RenderListBuilder::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);
    }

    std::optional<RenderList> RenderListBuilder::tryBuildRenderList(const std::string &name) {
        auto lock = std::lock_guard<std::mutex>(this->_mutex);

        auto viewEntity = tryGet(this->_renderListViewMap, name);

        if (!viewEntity.has_value()) {
            return std::nullopt;
        }

        std::optional<View> view;

        for (const auto &viewProvider: this->_viewProviders) {
            view = viewProvider->tryGetViewFor(*viewEntity);

            if (view.has_value()) {
                break;
            }
        }

        if (!view.has_value()) {
            return std::nullopt;
        }

        auto maybeDrawableEntities = this->discoverDrawables(*viewEntity);
        std::list<Drawable> drawables;

        if (!maybeDrawableEntities.has_value()) {
            return std::nullopt;
        }

        auto renderList = RenderList{
                .view = *view,
                .textureCount = 0,
                .textures = {},
                .meshes = {}
        };

        auto getTextureIdOf = [](RenderList &list, const std::string &asset) -> std::uint32_t {
            for (std::uint32_t id = 0; id < list.textureCount; ++id) {
                if (list.textures.at(id).asset == asset) {
                    return id;
                }
            }

            if (list.textureCount == TEXTURE_COUNT) {
                throw EngineError("Texture count overrun");
            }

            list.textures.at(list.textureCount).asset = asset;

            return list.textureCount++;
        };

        auto getMeshOf = [](RenderList &list, const std::string &asset) -> Mesh * {
            for (auto &mesh: list.meshes) {
                if (mesh.asset == asset) {
                    return &mesh;
                }
            }

            auto &mesh = list.meshes.emplace_back();
            mesh.asset = asset;

            return &mesh;
        };

        for (const auto &entity: (*maybeDrawableEntities)) {
            for (const auto &drawableProvider: this->_drawableProviders) {
                for (const auto &drawable: drawableProvider->getDrawablesFor(entity)) {
                    auto mesh = getMeshOf(renderList, drawable.meshAsset);

                    mesh->instances.emplace_back(MeshInstance{
                            .model = drawable.model,
                            .modelRot = drawable.modelRot,
                            .color = drawable.color,
                            .albedoTextureId = getTextureIdOf(renderList, drawable.albedoTextureAsset)
                    });
                }
            }
        }

        return renderList;
    }

    void RenderListBuilder::handleComponentCreate(const ECSEventArgs &eventArgs) {
        if (eventArgs.componentName != ViewComponent::name()) {
            return;
        }

        auto view = std::dynamic_pointer_cast<ViewComponent>(eventArgs.component);
        this->_renderListViewMap.insert_or_assign(view->getRenderList(), eventArgs.entity);
    }

    void RenderListBuilder::handleComponentDestroy(const ECSEventArgs &eventArgs) {
        if (eventArgs.componentName != ViewComponent::name()) {
            return;
        }

        auto entity = eventArgs.entity;
        auto it = std::find_if(this->_renderListViewMap.begin(), this->_renderListViewMap.end(),
                               [&entity](const auto &entry) {
                                   return entry.second == entity;
                               });

        if (it != this->_renderListViewMap.end()) {
            this->_renderListViewMap.erase(it);
        }
    }

    std::optional<std::set<Entity>> RenderListBuilder::discoverDrawables(const Entity &viewEntity) {
        auto maybeNode = this->_sceneManager->tryFindEntityNode(viewEntity);
        if (!maybeNode.has_value()) {
            return std::nullopt;
        }

        auto root = this->_sceneManager->getRoot(*maybeNode);

        std::set<Entity> drawableEntities;
        std::queue<SceneNodePtr> queue;
        queue.push(root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            for (const auto &descendant: current->getDescendants()) {
                queue.push(descendant);
            }

            if (current->getEntity().has_value()) {
                drawableEntities.insert(*current->getEntity());
            }
        }

        return drawableEntities;
    }
}
