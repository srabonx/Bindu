#ifndef BINDU_APPLAYERSTACK_H
#define BINDU_APPLAYERSTACK_H

#include <vector>
#include "AppLayer.h"

namespace BINDU
{

	using LayerIterator = std::vector<AppLayer*>::iterator;
	using RevLayerIterator = std::vector<AppLayer*>::reverse_iterator;

	using ConstLayerIterator = std::vector<AppLayer*>::const_iterator;
	using RevConstLayerIterator = std::vector<AppLayer*>::const_reverse_iterator;


	class AppLayerStack
	{
	public:
		AppLayerStack() = default;
		~AppLayerStack();

		void	PushLayer(AppLayer* appLayer);
		void	PopLayer(AppLayer* appLayer);

		LayerIterator begin();
		LayerIterator end();
		RevLayerIterator rbegin();
		RevLayerIterator rend();

		ConstLayerIterator begin() const;
		ConstLayerIterator end() const;
		RevConstLayerIterator rbegin() const;
		RevConstLayerIterator rend() const;

	private:
		std::vector<AppLayer*>	m_layers;
	};
}

#endif