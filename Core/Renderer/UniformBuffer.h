#ifndef BINDU_UNIFORM_BUFFER_H
#define BINDU_UNIFORM_BUFFER_H

#include <cstdint>


namespace BINDU
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, std::uint32_t size, std::uint32_t offset = 0) = 0;
	};
}

#endif