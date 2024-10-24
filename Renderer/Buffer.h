#ifndef BINDU_BUFFER_H
#define BINDU_BUFFER_H

#include <cstdint>
#include "DataFormat.h"


namespace BINDU
{
	class VertexBuffer
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
	};

	class IndexBuffer
	{
	public :
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;

		virtual std::uint32_t GetCount() const = 0;
	};
}

#endif