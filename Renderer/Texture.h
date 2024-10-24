#ifndef BINDU_TEXTURE_H
#define BINDU_TEXTURE_H

#include <cstdint>


namespace BINDU
{

	enum class TextureFormat
	{
		TYPELESS,
		RGBA8_UNORM,
		BGRA8_UNORM,
		D24_UNORM_S8_UINT
	};

	struct TextureSpecification
	{
		std::uint32_t Width{ 0 };
		std::uint32_t Height{ 0 };

		TextureFormat Format{ TextureFormat::RGBA8_UNORM };
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(std::uint32_t slot) = 0;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual std::uint32_t GetWidth() = 0;
		
		virtual std::uint32_t GetHeight() = 0;

		virtual bool	IsLoaded() = 0;
	};
}

#endif