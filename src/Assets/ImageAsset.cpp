#include <Penrose/Assets/ImageAsset.hpp>

namespace Penrose {

    Penrose::ImageAsset::ImageAsset(std::unique_ptr<Image> image)
            : _image(std::move(image)) {
        //
    }
}
