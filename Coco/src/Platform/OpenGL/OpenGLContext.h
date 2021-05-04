#pragma once
#include "Core/Base.h"
#include "Core/Graphics/GLContext.h"

#include "glad/glad.h"

namespace Coco
{
	class COCO_API OpenGLContext : public GLContext
	{
	public:
		static bool Loaded() { return s_Loaded; }

		virtual ~OpenGLContext() = default;

		virtual void Load(void* loaderAddress) override;
		virtual void Shutdown() override;
	private:
		static bool s_Loaded;
	};
}

