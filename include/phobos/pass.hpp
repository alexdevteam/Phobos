#pragma once

#include <string_view>
#include <vector>
#include <functional>

#include <vulkan/vulkan.h>

#include <plib/bit_flag.hpp>

#include <phobos/command_buffer.hpp>
#include <phobos/image.hpp>
#include <phobos/pipeline.hpp>
#include <phobos/buffer.hpp>

namespace ph {

struct ClearColor {
	float color[4]{ 0.0f, 0.0f, 0.0f, 0.1f };
};

struct ClearDepthStencil {
	float depth = 0.0f;
	uint32_t stencil = 0;
};

union ClearValue {
	ClearColor color;
	ClearDepthStencil depth_stencil;
};

enum class LoadOp {
	DontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	Load = VK_ATTACHMENT_LOAD_OP_LOAD,
	Clear = VK_ATTACHMENT_LOAD_OP_CLEAR
};


enum class ResourceAccess {
	ShaderRead = VK_ACCESS_SHADER_READ_BIT,
	ShaderWrite = VK_ACCESS_SHADER_WRITE_BIT,
	AttachmentOutput
};

enum class ResourceType {
	Image,
	Buffer,
	Attachment
};

struct ResourceUsage {
	plib::bit_flag<PipelineStage> stage{};
	ResourceAccess access{};

	struct Attachment {
		std::string name = "";
		LoadOp load_op = LoadOp::DontCare;
		ClearValue clear{ .color{} };
	};

	struct Image {
		ImageView view;
	};

	struct Buffer {
		BufferSlice slice;
	};

	Attachment attachment{};
	Image image{};
	Buffer buffer{};
	ResourceType type{};
};

struct Pass {
	// Resource usage data
	std::vector<ResourceUsage> resources{};
	// Name of this pass
	std::string name = "";
	// Execution callback
	std::function<void(ph::CommandBuffer&)> execute{};
};

// Utility class that helps creating render passes (ph::Pass).
// Usage:
// Create a PassBuilder using create(), set up the pass information using the member functions and finally call get() to obtain the final ph::Pass.
// You must properly register all attachments, buffers and storage images in order for automatic barriers to be inserted.
class PassBuilder {
public:
	static PassBuilder create(std::string_view name);

	// Adds an attachment to render to in this render pass.
	PassBuilder& add_attachment(std::string_view name, LoadOp load_op, ClearValue clear = { .color {} });
	// If you sample from an attachment that was rendered to in a previous pass, you must call this function to properly synchronize access and transition the image layout.
	PassBuilder& sample_attachment(std::string_view name, plib::bit_flag<PipelineStage> stage);
	// If you read from a buffer that was written to in an earlier pass, you must call this function to synchronize access automatically.
	PassBuilder& shader_read_buffer(BufferSlice slice, plib::bit_flag<PipelineStage> stage);
	// If you write to a buffer that will be read from in a later pass, you must call this function to synchronize access automatically.
	PassBuilder& shader_write_buffer(BufferSlice slice, plib::bit_flag<PipelineStage> stage);
	// Sets the execution callback for this render pass. Here you can record commands.
	PassBuilder& execute(std::function<void(ph::CommandBuffer&)> callback);

	Pass get();

private:
	Pass pass;
};

}