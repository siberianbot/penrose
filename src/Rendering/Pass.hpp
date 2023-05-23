#ifndef PENROSE_RENDERING_PASS_HPP
#define PENROSE_RENDERING_PASS_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class DeviceContext;
    
    class Pass {
    private:
        DeviceContext *_deviceContext;
        vk::RenderPass _renderPass;
        vk::Semaphore _semaphore;

    public:
        Pass(DeviceContext *deviceContext,
             const vk::RenderPass &renderPass,
             const vk::Semaphore &semaphore);
        ~Pass();

        [[nodiscard]] const vk::RenderPass &getRenderPass() const {
            return this->_renderPass;
        }

        [[nodiscard]] const vk::Semaphore &getSemaphore() const {
            return this->_semaphore;
        }
    };
}

#endif // PENROSE_RENDERING_PASS_HPP
