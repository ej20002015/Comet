#pragma once

#include "CometPCH.h"
#include "Layer.h"

namespace Comet
{

	//Not strictly a stack as new items can be inserted in the middle
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		//Two seperate parts of the stack - one for overlays (the latter portion) and one for traditional layers (from the start to the start of the overlays)
		//Overlays will always be after layers
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		//Iterators to move forward and backward through the stack
		std::vector<Layer*>::iterator begin()          { return m_layers.begin(); }
		std::vector<Layer*>::iterator end()            { return m_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		//Const iterators to move forward and backward through the stack
		std::vector<Layer*>::const_iterator begin() const          { return m_layers.begin(); }
		std::vector<Layer*>::const_iterator end() const            { return m_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const   { return m_layers.rend(); }

	private:
		std::vector<Layer*> m_layers;
		//Used to maintain the boundry between the traditional layers portion and the overlay portion of the vector/stack
		uint32_t m_layerInsertIndex = 0;
	};

}