@echo off

echo "Creating projects..."
pushd ..\
call vendor\premake\premake5.exe vs2019
popd

echo "Updating Git submodules..."
call git submodule update

echo "Make sure to download the VulkanSDK and install it."
echo "Also download the Vulkan Debug Libs here: https://sdk.lunarg.com/sdk/download/1.2.176.1/windows/VulkanSDK-1.2.176.1-DebugLibs.zip"
pause