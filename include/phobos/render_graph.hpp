#pragma once

#include <phobos/context.hpp>
#include <phobos/pass.hpp>

namespace ph {

class RenderGraph {
public:
	void add_pass(Pass pass);
	void build(Context& ctx);
private:
	friend class RenderGraphExecutor;

	struct BuiltPass {
		VkRenderPass handle = nullptr;
		VkFramebuffer framebuf = nullptr;
		VkExtent2D render_area{};
	};

	std::vector<Pass> passes;
	std::vector<BuiltPass> built_passes;

	std::vector<VkAttachmentDescription> get_attachment_descriptions(Context& ctx, Pass* pass);
	VkImageLayout get_initial_layout(Context& ctx, Pass* pass, PassOutput* attachment);
	VkImageLayout get_final_layout(Context& ctx, Pass* pass, PassOutput* attachment);

	struct AttachmentUsage {
		VkPipelineStageFlags stage{};
		VkAccessFlags access{};
		Pass* pass = nullptr;
	};

	AttachmentUsage find_previous_usage(Context& ctx, Pass* current_pass, Attachment* attachment);
	AttachmentUsage find_next_usage(Context& ctx, Pass* current_pass, Attachment* attachment);
};

class RenderGraphExecutor {
public:
	// Assumes .begin() has already been called on cmd_buf. Will not call .end() on it.
	// Render graph must be built before calling this function.
	void execute(ph::CommandBuffer& cmd_buf, RenderGraph& graph);
private:

};

}