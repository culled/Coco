project "ImGui"
location "../../CocoEngine/vendor/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../CocoEngine/vendor/imgui/imconfig.h",
        "../../CocoEngine/vendor/imgui/imgui.h",
        "../../CocoEngine/vendor/imgui/imgui.cpp",
		"../../CocoEngine/vendor/imgui/imgui_draw.cpp",
		"../../CocoEngine/vendor/imgui/imgui_internal.h",
		"../../CocoEngine/vendor/imgui/imgui_widgets.cpp",
		"../../CocoEngine/vendor/imgui/imstb_rectpack.h",
		"../../CocoEngine/vendor/imgui/imstb_textedit.h",
		"../../CocoEngine/vendor/imgui/imstb_truetype.h",
		"../../CocoEngine/vendor/imgui/imgui_demo.cpp",
        "../../CocoEngine/vendor/imgui/imgui_tables.cpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"