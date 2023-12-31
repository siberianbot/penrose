#ifndef PENROSE_RESOURCES_RESOURCE_GROUP_HPP
#define PENROSE_RESOURCES_RESOURCE_GROUP_HPP

namespace Penrose {

    /**
     * \brief Resource groups
     * \details Resource groups preserves order of resource initialization
     */
    enum class ResourceGroup {
        Engine,
        Performance,
        Backend,
        Windowing,
        Rendering,
        Assets,
        UI,
        Events,
        ECS,
        Scene,
        Custom
    };
}

#endif // PENROSE_RESOURCES_RESOURCE_GROUP_HPP
