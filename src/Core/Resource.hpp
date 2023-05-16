#ifndef PENROSE_CORE_RESOURCE_HPP
#define PENROSE_CORE_RESOURCE_HPP

namespace Penrose {

    class Resource {
    public:
        virtual ~Resource() = default;

        virtual void init() = 0;
        virtual void destroy() = 0;
    };
}

#endif // PENROSE_CORE_RESOURCE_HPP
