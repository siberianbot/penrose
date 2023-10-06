#ifndef PENROSE_RENDERING_RENDER_LIST_HPP
#define PENROSE_RENDERING_RENDER_LIST_HPP

#include <array>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include <Penrose/Rendering/Drawable.hpp>
#include <Penrose/Rendering/View.hpp>

namespace Penrose {

    // TODO: should be configurable
    constexpr static const std::uint32_t TEXTURE_COUNT = 64;

    struct Texture {
        std::string asset;
    };

    struct MeshInstance {
        glm::mat4 model;
        glm::mat4 modelRot;
        glm::vec3 color;
        std::uint32_t albedoTextureId;
    };

    struct Mesh {
        std::string asset;
        std::list<MeshInstance> instances;
    };

    struct RenderList {
        View view;

        std::uint32_t textureCount;
        std::array<Texture, TEXTURE_COUNT> textures;

        std::list<Mesh> meshes;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_HPP
