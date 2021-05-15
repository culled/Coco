#pragma once
#include "Core/Base.h"

#include "glm/glm.hpp"
#include "Camera.h"
#include "Textures.h"
#include "VertexArray.h"
#include "Material.h"
#include "MeshData.h"

namespace Coco
{
	struct COCO_API ShaderSceneData
	{
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
	};

	struct COCO_API ShaderModelData
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		float ID = -1.0f;
	};

	struct COCO_API SceneData
	{
		ShaderSceneData SceneData;
		ShaderModelData ModelData;
		Ref<UniformBuffer> SceneDataBuffer;
		Ref<UniformBuffer> ModelDataBuffer;
	};

	struct COCO_API BatchedVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		float ID;
		glm::vec4 Color;
		float TexID;
	};

	struct COCO_API BatchRenderData
	{
		static const size_t MaxTrianglesPerDrawcall = 10000;
		static const size_t MaxVerticiesPerDrawcall = MaxTrianglesPerDrawcall * 3;
		static const size_t MaxIndiciesPerDrawcall = MaxTrianglesPerDrawcall * 3;
		static const size_t MaxTextureSlots = 32; //TODO: render capabilities

		Ref<VertexBuffer> VertexBuffer = nullptr;
		Ref<IndexBuffer> IndexBuffer = nullptr;
		Ref<VertexArray> VertexArray = nullptr;

		std::array<BatchedVertex, MaxVerticiesPerDrawcall> VertexBase;
		BatchedVertex* VertexPtr = nullptr;
		size_t CurrentVertexCount = 0;

		std::array<uint32_t, MaxIndiciesPerDrawcall> Indicies;
		size_t CurrentIndexCount = 0;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		size_t TextureSlotIndex = 1; //0 = white texture

		Ref<Material> Material;
	};

	struct COCO_API RenderStats
	{
		size_t VerticiesDrawn = 0;
		size_t IndiciesDrawn = 0;
		size_t DrawCalls = 0;

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
		//using BatchMap = std::unordered_map<Ref<Material>, BatchRenderData>;
		//using BatchMapIterator = std::unordered_map<Ref<Material>, BatchRenderData>::iterator;

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform);
		static void EndScene();

		static void BeginBatch(const Ref<Material>& material);
		static void FlushBatch();

		static void SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform, int32_t ID = -1);
		static void SubmitMeshImmediate(const Ref<MeshData>& meshData, const Ref<Material>& material, const glm::mat4& transform, int32_t ID = -1);
		static void SubmitMeshBatched(const Ref<MeshData>& meshData, const glm::mat4& transform, int32_t ID = -1);

		static void ResetStats();
		static RenderStats GetStats() { return s_RenderStats; }

	private:
		static SceneData s_SceneData;
		static RenderStats s_RenderStats;
		static Ref<Texture2D> s_WhiteTexture;
		static BatchRenderData s_BatchData;

		friend class Renderer2D;
	};
}