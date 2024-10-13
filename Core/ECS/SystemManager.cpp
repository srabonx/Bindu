#include "SystemManager.h"

namespace BINDU
{
	void SystemManager::AddSystem(std::unique_ptr<ISystem>&& system)
	{
		m_systems.emplace_back(std::move(system));
	}

	void SystemManager::UpdateSystems(double dt)
	{
		for(auto& system : m_systems)
		{
			system->Update(dt);
		}
	}

	void SystemManager::RegisterEntity(EntityId entity)
	{
		for(auto& system : m_systems)
		{
			system->AddEntity(entity);
		}
	}

	void SystemManager::UnregisterEntity(EntityId entity)
	{
		for(auto& system : m_systems)
		{
			system->RemoveEntity(entity);
		}
	}
}