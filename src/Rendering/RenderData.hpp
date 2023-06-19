#ifndef PENROSE_RENDERING_RENDER_DATA_HPP
#define PENROSE_RENDERING_RENDER_DATA_HPP

#include <vector>

#include <glm/mat4x4.hpp>

#include "src/Assets/AssetId.hpp"

namespace Penrose {

    struct ModelData {
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 rotOnly;
    };

    struct RenderData {
        alignas(16) glm::mat4 matrix;
        alignas(16) ModelData model;
    };

    struct RenderListItem {
        AssetId mesh;
        RenderData data;
    };

    struct RenderList {
        std::vector<RenderListItem> items;
    };
}

#endif // PENROSE_RENDERING_RENDER_DATA_HPP
