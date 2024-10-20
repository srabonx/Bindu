#include "AppLayerStack.h"

namespace BINDU
{
	AppLayerStack::~AppLayerStack()
	{
		for(auto* layer : m_layers)
		{
			layer->OnDetach();

			delete layer;
		}
	}

	void AppLayerStack::PushLayer(AppLayer* appLayer)
	{
		appLayer->m_index = static_cast<std::uint16_t>(m_layers.size());

		appLayer->OnAttach();

		m_layers.emplace_back(appLayer);
	}

	void AppLayerStack::PopLayer(AppLayer* appLayer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), appLayer);

		if (it == m_layers.end()) return;

		appLayer->OnDetach();

		m_layers.erase(it);
	}

	LayerIterator AppLayerStack::begin()
	{
		return m_layers.begin();
	}
	LayerIterator AppLayerStack::end()
	{
		return m_layers.end();
	}
	RevLayerIterator AppLayerStack::rbegin()
	{
		return m_layers.rbegin();
	}
	RevLayerIterator AppLayerStack::rend()
	{
		return m_layers.rend();
	}
	ConstLayerIterator AppLayerStack::begin() const
	{
		return m_layers.begin();
	}
	ConstLayerIterator AppLayerStack::end() const
	{
		return m_layers.end();
	}
	RevConstLayerIterator AppLayerStack::rbegin() const
	{
		return m_layers.rbegin();
	}
	RevConstLayerIterator AppLayerStack::rend() const
	{
		return m_layers.rend();
	}
}