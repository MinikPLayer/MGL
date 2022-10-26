# CMakeList.txt : CMake project for MGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("lib")
include_directories("include")
include_directories("UserScripts")

# Add source to this project's executable.
add_executable (MGL 
	# Sources
    "glad.cpp"
    "Asset.cpp"
    "AssetsLoader.cpp"
    "Camera.cpp"
    "Color.cpp"
    "Cube.cpp"
    "Debug.cpp"
    "DirecitonalLight.cpp"
    "EngineUtil.cpp"
    "Framebuffer.cpp"
    "GameObject.cpp"
    "Input.cpp"
    "Light.cpp"
    "main.cpp"
    "Material.cpp"
    "Mesh.cpp"
    "MeshRenderer.cpp"
    "Model.cpp"
    "Path.cpp"
    "PlaneMesh.cpp"
    "PointLight.cpp"
    "PostProcessing.cpp"
    "Quaternion.cpp"
    "Shader.cpp"
    "ShadowMap.cpp"
    "Sky.cpp"
    "SystemInfo.cpp"
    "Texture.cpp"
    "TimeUtils.cpp"
    "Util.cpp"
    "Vector.cpp"
    "Window.cpp"
    
    # Includes
    "lib/glad/glad.h"
    "Asset.h"
        "AssetsLoader.h"
    "Camera.h"
    "Color.h"
    "Cube.h"
    "Debug.h"
    "DirecitonalLight.h"
    "EngineUtil.h"
    "Framebuffer.h"
    "GameObject.h"
    "Input.h"
    "Light.h"
    "Material.h"
    "Mesh.h"
    "MeshRenderer.h"
    "Model.h"
    "OBJLoader.h"
    "Path.h"
    "PlaneMesh.h"
    "PointLight.h"
    "PostProcessing.h"
    "Quaternion.h"
    "Shader.h"
    "ShadowMap.h"
    "Sky.h"
    "SystemInfo.h"
    "Texture.h"
    "TimeUtils.h"
    "Util.h"
    "Vector.h"
    "Window.h"

    # Cameras
    "Cameras/FlybackCamera.h"
    "Cameras/FlybackCamera.cpp"

    "UserScripts/UserScript.cpp"

    # Libraries
    #   Zlib
    "lib/zlib/miniz.cpp"
    "lib/zlib/miniz.h"
    #   ImGUI
    "lib/ImGui/backends/imgui_impl_glfw.cpp"
    "lib/ImGui/backends/imgui_impl_glfw.h"
    "lib/ImGui/backends/imgui_impl_opengl3.cpp"
    "lib/ImGui/backends/imgui_impl_opengl3.h"
    "lib/ImGui/backends/imgui_impl_opengl3_loader.h"
    "lib/ImGui/imconfig.h"
    "lib/ImGui/imgui.cpp"
    "lib/ImGui/imgui.h"
    "lib/ImGui/imgui_demo.cpp"
    "lib/ImGui/imgui_draw.cpp"
    "lib/ImGui/imgui_internal.h"
    "lib/ImGui/imgui_tables.cpp"
    "lib/ImGui/imgui_widgets.cpp"
    "lib/ImGui/imstb_rectpack.h"
    "lib/ImGui/imstb_textedit.h"
    "lib/ImGui/imstb_truetype.h"
    "lib/ImGui/misc/cpp/imgui_stdlib.cpp"
    "lib/ImGui/misc/cpp/imgui_stdlib.h"
    "lib/ImGui/misc/single_file/imgui_single_file.h"
    #   FBX
    "lib/FBX/fbxdocument.h"
    "lib/FBX/fbxnode.h"
    "lib/FBX/fbxproperty.h"
    "lib/FBX/fbxutil.h"
    
    "lib/FBX/fbxdocument.cpp"
    "lib/FBX/fbxdump.cpp"
    "lib/FBX/fbxnode.cpp"
    "lib/FBX/fbxproperty.cpp"
    "lib/FBX/fbxutil.cpp"
        UserScripts/Benchmark.cpp UserScripts/Benchmark.h SceneManager.cpp SceneManager.h)

find_package(glm CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE glfw)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE assimp)

add_definitions(-D GLM_ENABLE_EXPERIMENTAL)


set_property(TARGET MGL PROPERTY CXX_STANDARD 11)


# TODO: Add tests and install targets if needed.
