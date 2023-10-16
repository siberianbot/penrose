#ifndef PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP

#include <vector>
#include <string_view>

namespace Penrose {

    class VkInstanceExtensionsProvider {
    public:
        virtual ~VkInstanceExtensionsProvider() = default;

        [[nodiscard]] virtual std::vector<std::string_view> getRequiredInstanceExtensions() const = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
