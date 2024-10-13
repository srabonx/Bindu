#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <bitset>

namespace BINDU
{
	using EntitySig = std::uint64_t;


	struct IComponent
	{
	public:
		virtual ~IComponent() = default;

		EntitySig		Signature{ 0 };
	};

}

#endif