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
    "src/glad.cpp"
    "src/Asset.cpp"
    "src/AssetsLoader.cpp"
    "src/Camera.cpp"
    "src/Color.cpp"
    "src/Cube.cpp"
    "src/Debug.cpp"
    "src/DirecitonalLight.cpp"
    "src/EngineUtil.cpp"
    "src/Framebuffer.cpp"
    "src/GameObject.cpp"
    "src/Input.cpp"
    "src/Light.cpp"
    "src/main.cpp"
    "src/Material.cpp"
    "src/Mesh.cpp"
    "src/MeshRenderer.cpp"
    "src/Model.cpp"
    "src/Path.cpp"
    "src/PlaneMesh.cpp"
    "src/PointLight.cpp"
    "src/PostProcessing.cpp"
    "src/Quaternion.cpp"
    "src/Shader.cpp"
    "src/ShadowMap.cpp"
    "src/Sky.cpp"
    "src/SystemInfo.cpp"
    "src/Texture.cpp"
    "src/TimeUtils.cpp"
    "src/Util.cpp"
    "src/Vector.cpp"
    "src/Window.cpp" 
    
    # Includes
    "lib/glad/glad.h"
    "include/Asset.h"
    "include/Asset_b.h"
    "include/AssetsLoader.h"
    "include/Camera.h"
    "include/Color.h"
    "include/Cube.h"
    "include/Debug.h"
    "include/DirecitonalLight.h"
    "include/EngineUtil.h"
    "include/Framebuffer.h"
    "include/GameObject.h"
    "include/Input.h"
    "include/Light.h"
    "include/Material.h"
    "include/Mesh.h"
    "include/MeshRenderer.h"
    "include/Model.h"
    "include/OBJLoader.h"
    "include/Path.h"
    "include/PlaneMesh.h"
    "include/PointLight.h"
    "include/PostProcessing.h"
    "include/Quaternion.h"
    "include/Shader.h"
    "include/ShadowMap.h"
    "include/Sky.h"
    "include/SystemInfo.h"
    "include/Texture.h"
    "include/TimeUtils.h"
    "include/Util.h"
    "include/Vector.h"
    "include/Window.h"

    # Cameras
    "include/Cameras/FlybackCamera.h"
    "src/Cameras/FlybackCamera.cpp"

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
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE glm::glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE glfw)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(MGL PRIVATE assimp::assimp)


set_property(TARGET MGL PROPERTY CXX_STANDARD 11)


# TODO: Add tests and install targets if needed.
