#pragma once
#include "Core/Base.h"
#include "Layer.h"

namespace Coco
{
	class COCO_API LayerStack
	{
	public:
		virtual ~LayerStack();

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		void PopLayer(Ref<Layer>& layer);
		void PopOverlay(Ref <Layer>& overlay);

		inline std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		inline std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
		inline std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		inline std::vector<Ref<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }
	private:
		std::vector<Ref<Layer>> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}

