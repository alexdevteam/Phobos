#ifndef PHOBOS_PRESENT_FRAME_INFO_HPP_
#define PHOBOS_PRESENT_FRAME_INFO_HPP_

#include <vulkan/vulkan.hpp>
#include <stl/vector.hpp>

#include <phobos/renderer/mapped_ubo.hpp>
#include <phobos/renderer/instancing_buffer.hpp>
#include <phobos/renderer/render_attachment.hpp>
#include <phobos/renderer/render_target.hpp>
#include <phobos/pipeline/pipeline.hpp>

namespace ph {

class PresentManager;
class RenderGraph;

// TODO: Clean up this struct + make stuff private

struct FrameInfo {
    PresentManager* present_manager;
    RenderGraph* render_graph;

    size_t frame_index;
    size_t image_index;

    // Misc info
    size_t draw_calls;

    // Render target
    vk::Framebuffer framebuffer;
    vk::Image image;

    vk::Extent2D render_target_extent;

    // Main attachments for rendering
    RenderAttachment swapchain_color;

    RenderTarget swapchain_target;

    RenderTarget offscreen_target;

    // Synchronization
    vk::Fence fence;

    vk::Semaphore image_available;
    vk::Semaphore render_finished;

    // Other resources
    MappedUBO vp_ubo;
    InstancingBuffer transform_ssbo;
    MappedUBO lights;
    vk::Sampler default_sampler;

    // Descriptors
    vk::DescriptorSet fixed_descriptor_set;

    // Command buffers
    vk::CommandBuffer command_buffer;
    std::vector<vk::CommandBuffer> extra_command_buffers;

    // TODO: PerFrameCache class?
    Cache<vk::DescriptorSet, DescriptorSetBinding> descriptor_cache;
    vk::DescriptorPool descriptor_pool;
};

}

#endif