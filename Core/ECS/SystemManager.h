#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <memory>


#include "ISystem.h"

namespace BINDU
{
	class SystemManager
	{
	public:
		SystemManager() = default;
		~SystemManager() = default;

		// Add a system to the manager
		void	AddSystem(std::unique_ptr<ISystem>&& system);

		// Update all the systems in the manager
		void	UpdateSystems(double dt);

		// Register an Entity to all systems
		void	RegisterEntity(EntityId entity);

		// Unregister an Entity from all systems
		void	UnregisterEntity(EntityId entity);

	private:
		std::vector<std::unique_ptr<ISystem>>	m_systems;
	};
}

#endif