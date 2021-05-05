#pragma once
#include <Coco.h>

#include "glm/glm.hpp"

namespace Coco
{
	class InspectorPanel
	{
	public:
		void OnImGuiRender();

	private:
		static bool DrawVec3Control(const char* label, glm::vec3& value, float speed = 0.1f, glm::vec3 resetValue = glm::vec3(0.0f), float columnWidth = 100.0f);

		void DrawEntityComponents(Entity entity);

		template<typename T>
		void DrawComponent(const char* label, Entity entity, void(*DrawFunction)(T&))
		{
			if (!entity.HasComponent<T>()) return;

			if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
			{
				DrawFunction(entity.GetComponent<T>());
				ImGui::TreePop();
			}
		}
	};
}

