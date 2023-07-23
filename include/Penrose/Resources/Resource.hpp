#ifndef PENROSE_RESOURCES_RESOURCE_HPP
#define PENROSE_RESOURCES_RESOURCE_HPP

#include <type_traits>

namespace Penrose {

    class Resource {
    public:
        virtual ~Resource() = default;
    };

    class InitializableResource : public Resource {
    public:
        ~InitializableResource() override = default;

        virtual void init() = 0;
        virtual void destroy() = 0;
    };

    template<typename T>
    concept IsResource = std::is_base_of<Resource, T>::value;
}

#endif // PENROSE_RESOURCES_RESOURCE_HPP
