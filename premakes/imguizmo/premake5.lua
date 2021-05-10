project "ImGuizmo"
location "../../Coco/vendor/ImGuizmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../Coco/vendor/ImGuizmo/ImGuizmo.h",
        "../../Coco/vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "../../Coco/vendor/imgui"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"