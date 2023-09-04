#ifndef PENROSE_RESOURCES_INITIALIZABLE_HPP
#define PENROSE_RESOURCES_INITIALIZABLE_HPP

namespace Penrose {

    class Initializable {
    public:
        virtual ~Initializable() = default;

        virtual void init() = 0;
        virtual void destroy() = 0;
    };
}

#endif // PENROSE_RESOURCES_INITIALIZABLE_HPP