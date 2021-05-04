project "GLFW"
    location "../../Coco/vendor/glfw"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../Coco/vendor/glfw/include/GLFW/glfw3.h",
        "../../Coco/vendor/glfw/include/GLFW/glfw3native.h",
        "../../Coco/vendor/glfw/src/glfw_config.h",
        "../../Coco/vendor/glfw/src/context.c",
        "../../Coco/vendor/glfw/src/init.c",
        "../../Coco/vendor/glfw/src/input.c",
        "../../Coco/vendor/glfw/src/monitor.c",
        "../../Coco/vendor/glfw/src/vulkan.c",
        "../../Coco/vendor/glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "../../Coco/vendor/glfw/src/win32_init.c",
            "../../Coco/vendor/glfw/src/win32_joystick.c",
            "../../Coco/vendor/glfw/src/win32_monitor.c",
            "../../Coco/vendor/glfw/src/win32_time.c",
            "../../Coco/vendor/glfw/src/win32_thread.c",
            "../../Coco/vendor/glfw/src/win32_window.c",
            "../../Coco/vendor/glfw/src/wgl_context.c",
            "../../Coco/vendor/glfw/src/egl_context.c",
            "../../Coco/vendor/glfw/src/osmesa_context.c"
        }

        defines
        {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"