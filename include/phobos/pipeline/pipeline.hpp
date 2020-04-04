#ifndef PHOBOS_PIPELINE_HPP_
#define PHOBOS_PIPELINE_HPP_

#include <vulkan/vulkan.hpp>
#include <optional>
#include <unordered_map>
#include <string>

#include <stl/span.hpp>
#include <stl/vector.hpp>

#include <phobos/pipeline/pipelines.hpp>


namespace ph {

struct RenderPass;

struct DescriptorSetLayoutCreateInfo {
    stl::vector<vk::DescriptorSetLayoutBinding> bindings;
    // Optional. Leave empty to use default flags
    stl::vector<vk::DescriptorBindingFlags> flags;
};

struct DescriptorBinding {
    stl::size_t binding = 0;
    vk::DescriptorType type;

    union DescriptorContents {
        vk::DescriptorBufferInfo buffer;
        vk::DescriptorImageInfo image;
    };

    stl::vector<DescriptorContents> descriptors;
};

DescriptorBinding make_image_descriptor(uint32_t binding, vk::ImageView view, vk::Sampler sampler, vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal);
DescriptorBinding make_image_descriptor_array(uint32_t binding, stl::span<vk::ImageView> views, vk::Sampler sampler,
    vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal);
DescriptorBinding make_buffer_descriptor(uint32_t binding, vk::DescriptorType type, vk::Buffer buffer, vk::DeviceSize size, vk::DeviceSize offset = 0);

struct DescriptorSetBinding {
    stl::vector<DescriptorBinding> bindings;
    // Leave this on nullptr to use default pool
    vk::DescriptorPool pool = nullptr;
    // No need to manually set this field, the renderer can figure this out
    DescriptorSetLayoutCreateInfo set_layout;
};

struct PipelineLayoutCreateInfo {
    DescriptorSetLayoutCreateInfo set_layout;
    stl::vector<vk::PushConstantRange> push_constants;
};

struct PipelineLayout {
    vk::PipelineLayout  layout;
    vk::DescriptorSetLayout set_layout;
};

struct Pipeline {
    vk::Pipeline pipeline;
    PipelineLayout layout;

    std::string name;
};

struct PipelineCreateInfo {
    PipelineLayoutCreateInfo layout;

    vk::VertexInputBindingDescription vertex_input_binding;
    stl::vector<vk::VertexInputAttributeDescription> vertex_attributes;
    stl::vector<vk::PipelineShaderStageCreateInfo> shaders;
    vk::PipelineInputAssemblyStateCreateInfo input_assembly;
    vk::PipelineDepthStencilStateCreateInfo depth_stencil;
    stl::vector<vk::DynamicState> dynamic_states;
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    vk::PipelineMultisampleStateCreateInfo multisample;
    stl::vector<vk::PipelineColorBlendAttachmentState> blend_attachments;
    stl::vector<vk::Viewport> viewports;
    stl::vector<vk::Rect2D> scissors;
    bool blend_logic_op_enable;
    vk::GraphicsPipelineCreateInfo vk_info() const;
    
    void finalize();
private:
    friend class Renderer;
    friend struct std::hash<PipelineCreateInfo>;
    friend class RenderGraph;
    friend Pipeline create_or_get_pipeline(VulkanContext*, RenderPass*, PipelineCreateInfo);

    vk::PipelineVertexInputStateCreateInfo vertex_input;
    vk::PipelineColorBlendStateCreateInfo blending;
    vk::PipelineDynamicStateCreateInfo dynamic_state;
    vk::PipelineViewportStateCreateInfo viewport_state;

    // These are set before creating the actual VkPipeline
    vk::RenderPass render_pass;
    PipelineLayout pipeline_layout;
    stl::uint32_t subpass;
};

Pipeline create_or_get_pipeline(VulkanContext* ctx, RenderPass* pass, PipelineCreateInfo pci);

class PipelineManager {
public:
    void create_named_pipeline(std::string name, PipelineCreateInfo&& info);
    PipelineCreateInfo const* get_named_pipeline(std::string const& name) const;

    void destroy_all(vk::Device device);
private:
    std::unordered_map<std::string, PipelineCreateInfo> pipeline_infos;
};

}

#endif