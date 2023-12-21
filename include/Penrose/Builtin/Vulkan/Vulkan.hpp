#ifndef PENROSE_BUILTIN_VULKAN_VULKAN_HPP
#define PENROSE_BUILTIN_VULKAN_VULKAN_HPP

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    /**
     * \brief Configure Vulkan resources in provided resource set
     * \param resources Target resource set
     */
    void addVulkan(ResourceSet &resources);
}

#endif // PENROSE_BUILTIN_VULKAN_VULKAN_HPP
