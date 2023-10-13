#ifndef PENROSE_SCENE_SCENE_MANAGER_HPP
#define PENROSE_SCENE_SCENE_MANAGER_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>

#include <Penrose/Api.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Scene/SceneNode.hpp>

namespace Penrose {

    class PENROSE_API SceneManager : public Resource, public Initializable {
    public:
        ~SceneManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        [[nodiscard]] SceneNodePtr addRoot(std::string &&name);
        void removeRoot(std::string &&name);

        [[nodiscard]] SceneNodePtr getOrAddRoot(std::string &&name);

        [[nodiscard]] std::optional<SceneNodePtr> tryGetRoot(const std::string &name) const;
        [[nodiscard]] std::optional<SceneNodePtr> tryGetRoot(const SceneNodePtr &node) const;
        [[nodiscard]] SceneNodePtr getRoot(const std::string &name) const;
        [[nodiscard]] SceneNodePtr getRoot(const SceneNodePtr &node) const;

        [[nodiscard]] std::optional<std::string> tryGetRootNameOf(const SceneNodePtr &node) const;
        [[nodiscard]] std::string getRootNameOf(const SceneNodePtr &node) const;

        [[nodiscard]] std::optional<SceneNodePtr> tryFindEntityNode(const Entity &entity) const;
        [[nodiscard]] SceneNodePtr findEntityNode(const Entity &entity) const;

        SceneNodePtr insertEmptyNode(const SceneNodePtr &parent);
        SceneNodePtr insertEntityNode(const SceneNodePtr &parent, const Entity &entity);

        void moveNode(const SceneNodePtr &newParent, const SceneNodePtr &node);

        void removeNode(SceneNodePtr &&node, bool reparentDescendants);

    private:
        std::map<std::string, SceneNodePtr> _roots;

        [[nodiscard]] static std::optional<SceneNodePtr> tryFindEntityNode(const SceneNodePtr &root,
                                                                           const Entity &entity);
    };
}

#endif // PENROSE_SCENE_SCENE_MANAGER_HPP
