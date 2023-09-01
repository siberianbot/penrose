#ifndef PENROSE_RESOURCES_UPDATABLE_HPP
#define PENROSE_RESOURCES_UPDATABLE_HPP

namespace Penrose {

    class Updatable {
    public:
        virtual ~Updatable() = default;

        virtual void update(float delta) = 0;
    };
}

#endif // PENROSE_RESOURCES_UPDATABLE_HPP
