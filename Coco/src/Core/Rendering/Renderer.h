#pragma once
#include "Core/Base.h"

#include "glm/glm.hpp"
#include "Camera.h"
#include "Textures.h"
#include "VertexArray.h"
#include "Material.h"

namespace Coco
{
	struct COCO_API SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	struct COCO_API BatchVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;
	};

	struct COCO_API BatchRenderData
	{
		static const uint32_t MaxTrianglesPerDrawcall = 10000;
		static const uint32_t MaxVerticiesPerDrawcall = MaxTrianglesPerDrawcall * 3;
		static const uint32_t MaxIndiciesPerDrawcall = MaxTrianglesPerDrawcall * 3;
		static const uint32_t MaxTextureSlots = 32; //TODO: render capabilities

		uint32_t IndexCount = 0;
		BatchVertex* VertexBufferBase = nullptr;
		BatchVertex* VertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; //0 = white texture
	};

	struct COCO_API RenderStats
	{
		uint32_t VerticiesDrawn = 0;
		uint32_t IndiciesDrawn = 0;
		uint32_t DrawCalls = 0;

		void StartTimer()
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		void EndTimer()
		{
			auto endTime = std::chrono::high_resolution_clock::now();
			m_Duration = endTime - m_StartTime;
		}

		float GetDuration()
		{
			return m_Duration.count();
		}

		float GetDurationMilliseconds()
		{
			return m_Duration.count() * 1000.0f;
		}

	private:
		std::chrono::high_resolution_clock::time_point m_StartTime;
		std::chrono::duration<float> m_Duration;
	};

	class COCO_API Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform);
		static void EndScene();

		static void SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform);

		static void ResetStats();
		static RenderStats GetStats() { return s_RenderStats; }

	private:
		static SceneData s_SceneData;
		static RenderStats s_RenderStats;
	};
}