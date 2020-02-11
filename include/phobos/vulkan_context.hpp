#ifndef PHOBOS_VULKAN_CONTEXT_HPP_
#define PHOBOS_VULKAN_CONTEXT_HPP_

#include <vulkan/vulkan.hpp>

#include <phobos/window_context.hpp>
#include <phobos/version.hpp>
#include <phobos/physical_device.hpp>

namespace ph {

struct VulkanContext {
    vk::Instance instance;    
    vk::DispatchLoaderDynamic dynamic_dispatcher;
    // Only available if enable_validation_layers is set to true
    vk::DebugUtilsMessengerEXT debug_messenger;

    vk::SurfaceKHR surface;

    PhysicalDeviceDetails physical_device;

    void destroy();
};

struct AppSettings {
    bool enable_validation_layers = false;
    Version version = Version {1, 0, 0};
};

VulkanContext create_vulkan_context(WindowContext const& window_ctx, AppSettings settings = {});

}

#endif