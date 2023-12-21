#include "VkRenderSystem.hpp"

#include "src/Builtin/Vulkan/Rendering/VkRenderContext.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkRenderSystem";

    inline constexpr std::uint32_t MAX_SET_COUNT = 64 * 1024;
    inline constexpr std::uint32_t MAX_DESCRIPTOR_COUNT = 1024;

    VkRenderSystem::VkRenderSystem(const ResourceSet *resources)
        : _resources(resources),
          _log(resources->get<Log>()),
          _surfaceManager(resources->get<SurfaceManager>()),
          _physicalDeviceSelector(resources->get<VkPhysicalDeviceSelector>()),
          _logicalDeviceFactory(resources->get<VkLogicalDeviceFactory>()),
          _bufferFactory(resources->get<VkBufferFactory>()),
          _imageFactory(resources->get<VkImageFactory>()),
          _pipelineFactory(resources->get<VkPipelineFactory>()),
          _swapchainFactory(resources->get<VkSwapchainFactory>()) {
        //
    }

    void VkRenderSystem::init() {
        try {
            this->_physicalDevice = this->_physicalDeviceSelector->selectPhysicalDevice();
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to select physical device"));
        }

        this->_log->writeInfo(
            TAG, "Chosen device: device id = {:#x}, device name = {}, driver version = {:#x}",
            this->_physicalDevice->properties.deviceID,
            static_cast<char *>(this->_physicalDevice->properties.deviceName),
            this->_physicalDevice->properties.driverVersion
        );

        try {
            this->_logicalDevice = this->_logicalDeviceFactory->makeLogicalDevice(*this->_physicalDevice);
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to create logical device"));
        }

        this->_bufferFactory->init();
        this->_imageFactory->init();
        this->_pipelineFactory->init();
    }

    void VkRenderSystem::destroy() {
        this->_pipelineFactory->destroy();
        this->_imageFactory->destroy();
        this->_bufferFactory->destroy();

        this->_logicalDevice = std::nullopt;
    }

    RenderContext *VkRenderSystem::makeRenderContext() {
        const auto surface = this->_surfaceManager->getSurface();
        auto swapchain = this->_swapchainFactory->makeSwapchain(surface);

        const auto commandPoolCreateInfo = vk::CommandPoolCreateInfo()
                                               .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                               .setQueueFamilyIndex(this->_physicalDevice->graphicsFamilyIdx);

        auto commandPool = this->_logicalDevice->handle->createCommandPoolUnique(commandPoolCreateInfo);

        const auto descriptorPoolSizes = {
            vk::DescriptorPoolSize(vk::DescriptorType::eSampler, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, MAX_DESCRIPTOR_COUNT),
            vk::DescriptorPoolSize(vk::DescriptorType::eInlineUniformBlock, MAX_DESCRIPTOR_COUNT),
        };

        const auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo()
                                                  .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
                                                  .setMaxSets(MAX_SET_COUNT)
                                                  .setPoolSizes(descriptorPoolSizes);

        auto descriptorPool = this->_logicalDevice->handle->createDescriptorPoolUnique(descriptorPoolCreateInfo);

        const auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                                                   .setLevel(vk::CommandBufferLevel::ePrimary)
                                                   .setCommandPool(commandPool.get())
                                                   .setCommandBufferCount(INFLIGHT_FRAME_COUNT);

        auto commandBuffers = this->_logicalDevice->handle->allocateCommandBuffersUnique(commandBufferAllocateInfo);

        std::array<VkRenderContext::FrameData, INFLIGHT_FRAME_COUNT> frameData;
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; ++idx) {
            frameData[idx] = VkRenderContext::FrameData {
                .fence = this->_logicalDevice->handle->createFenceUnique(
                    vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled)
                ),
                .commandBuffer = std::move(commandBuffers.at(idx)),
                .imageReady = this->_logicalDevice->handle->createSemaphoreUnique(vk::SemaphoreCreateInfo()),
                .renderFinished = this->_logicalDevice->handle->createSemaphoreUnique(vk::SemaphoreCreateInfo())
            };
        }

        return new VkRenderContext(
            this->_log, this->_resources->get<VkLogicalDeviceProvider>(),
            this->_resources->get<VkInternalObjectFactory>(), this->_imageFactory, this->_pipelineFactory,
            this->_swapchainFactory, std::move(swapchain), std::move(commandPool), std::move(descriptorPool),
            std::move(frameData)
        );
    }
}
