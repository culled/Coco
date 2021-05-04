#pragma once

#include "Core/Base.h"

namespace Coco
{
	class COCO_API GLContext
	{
	public:
		static void Create();

		virtual ~GLContext() = default;

		virtual void Load(void* loaderAddress) = 0;
		virtual void Shutdown() = 0;

		static Ref<GLContext> GetCurrentContext() { return s_Context; }

	private:
		static Ref<GLContext> s_Context;
	};
}