#include "ccpch.h"
#include "LayerStack.h"

namespace Coco
{
	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetached();
		}
	}

	void LayerStack::PushLayer(Ref<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttached();
	}

	void LayerStack::PushOverlay(Ref<Layer> overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttached();
	}

	void LayerStack::PopLayer(Ref<Layer>& layer)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [&](Ref<Layer> const& p) {
			return p.get() == layer.get();
			});

		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetached();
		}
	}

	void LayerStack::PopOverlay(Ref<Layer>& overlay)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [&](Ref<Layer> const& p) {
			return p.get() == overlay.get();
			});

		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			overlay->OnDetached();
		}
	}
}