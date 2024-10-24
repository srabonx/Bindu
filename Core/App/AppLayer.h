
#ifndef BINDU_APPLAYER_H
#define BINDU_APPLAYER_H


#include <string>
#include <cstdint>
#include "../../Event/EventStruct.h"


namespace BINDU
{
	class CommandList;

	class AppLayer
	{
		friend class AppLayerStack;
	public:
		AppLayer(std::string name = "Layer");
		virtual ~AppLayer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(double dt) = 0;
		virtual void OnRender(CommandList& commandList) = 0;

		virtual void OnEvent(EVENT::BND_Event& event) = 0;

		const std::string& GetName() const { return m_name; }

		std::uint16_t GetIndex() const { return m_index; }

	protected:
		std::string		m_name;
		std::uint16_t	m_index{ 0 };
	};
}

#endif