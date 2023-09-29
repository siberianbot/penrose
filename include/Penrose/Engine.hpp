#ifndef PENROSE_ENGINE_HPP
#define PENROSE_ENGINE_HPP

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class Engine {
    private:
        ResourceSet _resources;

    public:
        Engine();

        void run();

        [[nodiscard]] ResourceSet &resources() { return this->_resources; }
    };
}

#endif // PENROSE_ENGINE_HPP
