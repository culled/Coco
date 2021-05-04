project "GLFW"
    location "../../CocoEngine/vendor/glfw"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../CocoEngine/vendor/glfw/include/GLFW/glfw3.h",
        "../../CocoEngine/vendor/glfw/include/GLFW/glfw3native.h",
        "../../CocoEngine/vendor/glfw/src/glfw_config.h",
        "../../CocoEngine/vendor/glfw/src/context.c",
        "../../CocoEngine/vendor/glfw/src/init.c",
        "../../CocoEngine/vendor/glfw/src/input.c",
        "../../CocoEngine/vendor/glfw/src/monitor.c",
        "../../CocoEngine/vendor/glfw/src/vulkan.c",
        "../../CocoEngine/vendor/glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "../../CocoEngine/vendor/glfw/src/win32_init.c",
            "../../CocoEngine/vendor/glfw/src/win32_joystick.c",
            "../../CocoEngine/vendor/glfw/src/win32_monitor.c",
            "../../CocoEngine/vendor/glfw/src/win32_time.c",
            "../../CocoEngine/vendor/glfw/src/win32_thread.c",
            "../../CocoEngine/vendor/glfw/src/win32_window.c",
            "../../CocoEngine/vendor/glfw/src/wgl_context.c",
            "../../CocoEngine/vendor/glfw/src/egl_context.c",
            "../../CocoEngine/vendor/glfw/src/osmesa_context.c"
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