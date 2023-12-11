#ifndef PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP

#include <string_view>
#include <vector>

namespace Penrose {

    /**
     * \brief Interface of Vulkan instance extensions provider
     */
    class VkInstanceExtensionsProvider {
    public:
        virtual ~VkInstanceExtensionsProvider() = default;

        /**
         * \brief Get list of requred instance extensions
         * \return List of required instance extensions
         */
        [[nodiscard]] virtual std::vector<std::string_view> getRequiredInstanceExtensions() const = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_INSTANCE_EXTENSIONS_PROVIDER_HPP
