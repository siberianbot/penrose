#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP

#include <array>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    class VkDescriptorPoolManager final: public Resource<VkDescriptorPoolManager> {
    public:
        using DescriptorSets = std::array<vk::DescriptorSet, INFLIGHT_FRAME_COUNT>;

        explicit VkDescriptorPoolManager(const ResourceSet *resources);
        ~VkDescriptorPoolManager() override = default;

        void init();
        void destroy();

        [[nodiscard]] const vk::DescriptorPool &getDescriptorPool() const { return this->_state->descriptorPool; }

        [[nodiscard]] DescriptorSets createDescriptorSets(const vk::DescriptorSetLayout &layout);

        void destroyDescriptorSets(const DescriptorSets &descriptorSets);

    private:
        struct State {
            vk::DescriptorPool descriptorPool;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;

        std::optional<State> _state;

        [[nodiscard]] State createDescriptorPool();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP
