#include "Pass.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    Pass::Pass(DeviceContext *deviceContext,
               const vk::RenderPass &renderPass,
               const vk::Semaphore &semaphore)
            : _deviceContext(deviceContext),
              _renderPass(renderPass),
              _semaphore(semaphore) {
        //
    }

    Pass::~Pass() {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        logicalDevice.destroy(this->_semaphore);
        logicalDevice.destroy(this->_renderPass);
    }
}
