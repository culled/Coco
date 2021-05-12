project "GLFW"
    location "%{wks.location}/Coco/vendor/glfw"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Coco/vendor/glfw/include/GLFW/glfw3.h",
        "%{wks.location}/Coco/vendor/glfw/include/GLFW/glfw3native.h",
        "%{wks.location}/Coco/vendor/glfw/src/glfw_config.h",
        "%{wks.location}/Coco/vendor/glfw/src/context.c",
        "%{wks.location}/Coco/vendor/glfw/src/init.c",
        "%{wks.location}/Coco/vendor/glfw/src/input.c",
        "%{wks.location}/Coco/vendor/glfw/src/monitor.c",
        "%{wks.location}/Coco/vendor/glfw/src/vulkan.c",
        "%{wks.location}/Coco/vendor/glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "%{wks.location}/Coco/vendor/glfw/src/win32_init.c",
            "%{wks.location}/Coco/vendor/glfw/src/win32_joystick.c",
            "%{wks.location}/Coco/vendor/glfw/src/win32_monitor.c",
            "%{wks.location}/Coco/vendor/glfw/src/win32_time.c",
            "%{wks.location}/Coco/vendor/glfw/src/win32_thread.c",
            "%{wks.location}/Coco/vendor/glfw/src/win32_window.c",
            "%{wks.location}/Coco/vendor/glfw/src/wgl_context.c",
            "%{wks.location}/Coco/vendor/glfw/src/egl_context.c",
            "%{wks.location}/Coco/vendor/glfw/src/osmesa_context.c"
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