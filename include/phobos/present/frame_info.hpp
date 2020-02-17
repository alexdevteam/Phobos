#ifndef PHOBOS_PRESENT_FRAME_INFO_HPP_
#define PHOBOS_PRESENT_FRAME_INFO_HPP_

#include <vulkan/vulkan.hpp>
#include <vector>

#include <phobos/renderer/mapped_ubo.hpp>

namespace ph {

struct FrameInfo {
    size_t frame_index;
    size_t image_index;

    // Misc info
    size_t draw_calls;

    // Render target
    vk::Framebuffer framebuffer;
    vk::Image image;

    // Synchronization
    vk::Fence fence;

    vk::Semaphore image_available;
    vk::Semaphore render_finished;

    // Other resources
    MappedUBO mvp_ubo;

    vk::DescriptorSet fixed_descriptor_set;

    // Command buffers
    vk::CommandBuffer command_buffer;
    std::vector<vk::CommandBuffer> extra_command_buffers;
};

}

#endif