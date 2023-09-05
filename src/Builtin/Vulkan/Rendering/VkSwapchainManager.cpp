#include "VkSwapchainManager.hpp"

#include <string_view>

#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    constexpr static const std::string_view VK_SWAPCHAIN_MANAGER_TAG = "VkSwapchainManager";

    VkSwapchainManager::VkSwapchainManager(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _swapchainFactory(resources->getLazy<VkSwapchainFactory>()) {
        //
    }

    void VkSwapchainManager::init() {
        this->_log->writeInfo(VK_SWAPCHAIN_MANAGER_TAG, "Initializing swapchain manager");

        auto swapchain = this->_swapchainFactory->makeSwapchain();

        this->_swapchain = std::unique_ptr<VkSwapchain>(swapchain);
    }

    void VkSwapchainManager::destroy() {
        this->_swapchain = std::nullopt;
    }

    void VkSwapchainManager::recreate() {
        this->_log->writeInfo(VK_SWAPCHAIN_MANAGER_TAG, "Recreating swapchain");

        auto swapchain = this->_swapchainFactory->makeSwapchain(
                map(this->_swapchain, [](const std::unique_ptr<VkSwapchain> &swapchain) {
                    return swapchain.get();
                }));

        this->_swapchain = std::unique_ptr<VkSwapchain>(swapchain);
    }
}
