#ifndef CBALLOCATION_H
#define CBALLOCATION_H

#include <memory>
#include "../Renderer/DirectX/VariableSizeMemoryAllocator.h"

namespace BINDU
{
	struct CbAllocation
	{
		using Allocator = std::weak_ptr<VariableSizeMemoryAllocator>;

		std::uint64_t					Index{ 0 };

		Allocator						ParentAllocator;
	};
}

#endif