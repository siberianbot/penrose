#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP

#include <array>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkDescriptorPoolManager : public Resource, public Initializable {
    public:
        using DescriptorSets = std::array<vk::DescriptorSet, INFLIGHT_FRAME_COUNT>;

        explicit VkDescriptorPoolManager(ResourceSet *resources);
        ~VkDescriptorPoolManager() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] const vk::DescriptorPool &getDescriptorPool() const { return this->_state->descriptorPool; }

        [[nodiscard]] DescriptorSets createDescriptorSets(const vk::DescriptorSetLayout &layout);

        void destroyDescriptorSets(const DescriptorSets &descriptorSets);

    private:
        struct State {
            vk::DescriptorPool descriptorPool;
        };

        Lazy<Log> _log;
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;

        std::optional<State> _state;

        [[nodiscard]] State createDescriptorPool();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_POOL_MANAGER_HPP
