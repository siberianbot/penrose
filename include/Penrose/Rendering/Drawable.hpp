#ifndef PENROSE_RENDERING_DRAWABLE_HPP
#define PENROSE_RENDERING_DRAWABLE_HPP

#include <string>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    struct Drawable {
        Entity entity;
        std::string meshAsset;
        std::string albedoTextureAsset;
        glm::mat4 model;
        glm::mat4 modelRot;
    };
}

#endif // PENROSE_RENDERING_DRAWABLE_HPP
