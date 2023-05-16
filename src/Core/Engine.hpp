#ifndef PENROSE_CORE_ENGINE_HPP
#define PENROSE_CORE_ENGINE_HPP

#include "src/Core/ResourceSet.hpp"

namespace Penrose {

    class Engine {
    private:
        ResourceSet _resources;

    public:
        void run();

        [[nodiscard]] ResourceSet &resources() { return this->_resources; }
    };
}

#endif // PENROSE_CORE_ENGINE_HPP
