#include "VkLogicalDeviceFactory.hpp"

#include <algorithm>
#include <cstdint>
#include <set>
#include <string_view>
#include <vector>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    // ReSharper disable once CppMemberFunctionMayBeStatic
    VkLogicalDevice VkLogicalDeviceFactory::makeLogicalDevice(const VkPhysicalDevice &physicalDevice) {
        auto defaultQueuePriorities = {1.0f};
        auto uniqueQueueFamilies = std::set {
            physicalDevice.graphicsFamilyIdx,
            physicalDevice.transferFamilyIdx,
            physicalDevice.presentFamilyIdx,
        };

        auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>(uniqueQueueFamilies.size());
        std::ranges::transform(
            uniqueQueueFamilies, queueCreateInfos.begin(),
            [&defaultQueuePriorities](const std::uint32_t &familyIdx) {
                return vk::DeviceQueueCreateInfo().setQueueFamilyIndex(familyIdx).setQueuePriorities(
                    defaultQueuePriorities
                );
            }
        );

        // TODO: enabled features should be specified here
        constexpr auto enabledFeatures = vk::PhysicalDeviceFeatures();

        auto enabledExtensions = std::vector<const char *>(REQUIRED_DEVICE_EXTENSIONS.size());
        std::ranges::transform(
            REQUIRED_DEVICE_EXTENSIONS, enabledExtensions.begin(),
            [](const std::string_view &extension) { return extension.data(); }
        );

        const auto createInfo = vk::DeviceCreateInfo()
                                    .setQueueCreateInfos(queueCreateInfos)
                                    .setPEnabledFeatures(&enabledFeatures)
                                    .setPEnabledExtensionNames(enabledExtensions);

        auto handle = physicalDevice.handle.createDeviceUnique(createInfo);

        auto graphicsQueue = handle->getQueue(physicalDevice.graphicsFamilyIdx, 0);
        auto transferQueue = handle->getQueue(physicalDevice.transferFamilyIdx, 0);
        auto presentQueue = handle->getQueue(physicalDevice.presentFamilyIdx, 0);

        return {
            .handle = std::move(handle),
            .graphicsQueue = graphicsQueue,
            .transferQueue = transferQueue,
            .presentQueue = presentQueue,
        };
    }
}
