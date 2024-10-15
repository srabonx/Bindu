#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ISystem.h"
#include "Components.h"
#include "ComponentManager.h"
#include <debugapi.h>

namespace BINDU
{
	class NameSystem : public ISystem
	{
	public:
		NameSystem(ComponentManager& manager) : m_componentManager(manager) {}

		void Update(double dt) override
		{
			for (auto& e : m_entities)
			{
				if (m_componentManager.HasComponent<NameComponent>(e))
				{
					auto compo = m_componentManager.GetComponent<NameComponent>(e);

					OutputDebugStringA(compo->Name.c_str());
					OutputDebugStringA("\n");
				}
			}
		}

	private:
		ComponentManager& m_componentManager;
	};
}

#endif