project "ImGuizmo"
location "%{wks.location}/Coco/vendor/ImGuizmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Coco/vendor/ImGuizmo/ImGuizmo.h",
        "%{wks.location}/Coco/vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "%{wks.location}/Coco/vendor/imgui"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"