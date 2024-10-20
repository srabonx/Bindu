#ifndef BINDU_COMMAND_QUEUE_H
#define BINDU_COMMAND_QUEUE_H

#include "RenderCommand.h"
#include <vector>


namespace BINDU
{
	class CommandQueue
	{
	public:
		CommandQueue() = default;
		~CommandQueue() = default;

		void AddCommand(RenderCommand* command)
		{
			m_commands.emplace_back(command);
		}

		void ExecuteCommands()
		{
			for (auto& it = m_commands.begin(); it != m_commands.end(); ++it)
			{
				(*it)->Execute();

				delete (*it);

				it = m_commands.erase(it);
			}

			m_commands.clear();
		}

	private:
		std::vector<RenderCommand*> m_commands;
	};
}

#endif