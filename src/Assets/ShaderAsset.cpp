#include <Penrose/Assets/ShaderAsset.hpp>

namespace Penrose {

    ShaderAsset::ShaderAsset(std::unique_ptr <Shader> shader)
            : _shader(std::move(shader)) {
        //
    }
}
