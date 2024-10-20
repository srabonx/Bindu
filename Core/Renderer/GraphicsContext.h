#ifndef BINDU_GRAPHICS_CONTEXT_H
#define BINDU_GRAPHICS_CONTEXT_H

#include "Texture.h"

namespace BINDU
{
	class CommandList;
}

namespace BINDU
{
	struct GraphicsContextProperties
	{
		std::uint16_t	BackBufferCount{ 2 };

		TextureFormat	BackBufferFormat{ TextureFormat::RGBA8_UNORM };

		std::uint16_t	MSAASampleCount{ 1 };

		std::uint16_t	MSAASampleQuality{ 0 };
	};

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Initialize(const GraphicsContextProperties& properties) = 0;

		virtual void PrepareForCommands() = 0;

		virtual Ref<CommandList> GetCommandList() = 0;

		virtual void ExecuteCommands() = 0;

		virtual void Present() = 0;

		virtual void Resize(std::uint16_t newWidth, std::uint16_t newHeight) = 0;
	};
}

#endif