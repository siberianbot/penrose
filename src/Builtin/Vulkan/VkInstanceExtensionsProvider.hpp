#ifndef PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP

#include <vector>

namespace Penrose {

    class VkInstanceExtensionsProvider {
    public:
        virtual ~VkInstanceExtensionsProvider() = default;

        [[nodiscard]] virtual std::vector<const char *> getRequiredInstanceExtensions() const = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
