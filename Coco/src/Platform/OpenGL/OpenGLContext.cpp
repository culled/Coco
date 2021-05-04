#include "ccpch.h"
#include "OpenGLContext.h"
#include "Core/App/Log.h"

namespace Coco
{
	bool OpenGLContext::s_Loaded = false;

	void OpenGLContext::Load(void* loaderAddress)
	{
		int gladStatus = gladLoadGLLoader((GLADloadproc)loaderAddress);
		ASSERT_CORE(gladStatus, "Failed to initialize Glad");

#ifdef ENABLE_ASSERTS
		int vMajor, vMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &vMajor);
		glGetIntegerv(GL_MINOR_VERSION, &vMinor);

		ASSERT_CORE(vMajor > 4 || (vMajor == 4 && vMinor >= 5), "CocoEngine only supports OpenGL 4.5+");
#endif

		LOG_CORE_INFO("Initialized OpenGL context:");
		LOG_CORE_INFO("-- Vendor: {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("-- Version: {0}", glGetString(GL_VERSION));

		s_Loaded = true;
	}

	void OpenGLContext::Shutdown()
	{
	}
}
