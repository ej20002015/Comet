#include "CometPCH.h"
#include "LayerStack.h"

namespace Comet
{

	//Layers are only destroyed when the layer stack is destroyed.
	//Layers are not destroyed when a layer is popped. The layer stack
	//belongs to the application and is only destroyed when the application
	//is destroyed so dangling pointers should not be a problem.
	Comet::LayerStack::~LayerStack()
	{
		for (auto layer : m_layers)
			delete layer;
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		layer->onAttach();
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
		if (it != m_layers.begin() + m_layerInsertIndex)
		{
			layer->onDetach();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		overlay->onAttach();
		m_layers.emplace_back(overlay);
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->onDetach();
			m_layers.erase(it);
		}
	}

}
