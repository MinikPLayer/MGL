set(PROJECT_NAME GLTemplate)

################################################################################
# Source groups
################################################################################
set(Libraries__ImGui
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
)
source_group("Libraries\\ImGui" FILES ${Libraries__ImGui})

set(Libraries__zlib
    "zlib/miniz.cpp"
    "zlib/miniz.h"
)
source_group("Libraries\\zlib" FILES ${Libraries__zlib})

set(Pliki_nagwkowe
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
    "include/Time.h"
    "include/Util.h"
    "include/Vector.h"
    "include/Window.h"
)
include_directories("lib")
include_directories("include")
include_directories("UserScripts")
source_group("Pliki nagłówkowe" FILES ${Pliki_nagwkowe})

set(Pliki_nagwkowe__Cameras
    "include/Cameras/FlybackCamera.h"
)
source_group("Pliki nagłówkowe\\Cameras" FILES ${Pliki_nagwkowe__Cameras})

set(Pliki_nagwkowe__Editor
    "include/Editor/Editor.h"
)
source_group("Pliki nagłówkowe\\Editor" FILES ${Pliki_nagwkowe__Editor})

set(Pliki_nagwkowe__FBX
    "lib/FBX/fbxdocument.h"
    "lib/FBX/fbxnode.h"
    "lib/FBX/fbxproperty.h"
    "lib/FBX/fbxutil.h"
)
source_group("Pliki nagłówkowe\\FBX" FILES ${Pliki_nagwkowe__FBX})

set(Pliki_nagwkowe__UserScripts
    "UserScripts/UserScript.h"
)
source_group("Pliki nagłówkowe\\UserScripts" FILES ${Pliki_nagwkowe__UserScripts})

set(Pliki_zasobw
    "Assets/Shaders/DepthMapShader.frag"
    "Assets/Shaders/DepthMapShader.vert"
    "Assets/Shaders/FragmentLightShader.frag"
    "Assets/Shaders/FragmentShader.frag"
    "Assets/Shaders/SkyShader.frag"
    "Assets/Shaders/SkyShader.vert"
    "Assets/Shaders/SkyShader2.frag"
    "Assets/Shaders/SkyShader2.vert"
    "Assets/Shaders/VertexLightShader.vert"
    "Assets/Shaders/VertexShader.vert"
    "atmosphere.glsl"
)
source_group("Pliki zasobów" FILES ${Pliki_zasobw})

set(Pliki_zasobw__Engine
    "Assets/Shaders/PP.frag"
    "Assets/Shaders/PP.vert"
    "Assets/Shaders/ShadowMapFragmentShader.frag"
    "Assets/Shaders/ShadowMapVertexShader.vert"
)
source_group("Pliki zasobów\\Engine" FILES ${Pliki_zasobw__Engine})

set(Pliki_rdowe
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
    "src/Time.cpp"
    "src/Util.cpp"
    "src/Vector.cpp"
    "src/Window.cpp"
)
source_group("Pliki źródłowe" FILES ${Pliki_rdowe})

set(Pliki_rdowe__Cameras
    "src/Cameras/FlybackCamera.cpp"
)
source_group("Pliki źródłowe\\Cameras" FILES ${Pliki_rdowe__Cameras})

set(Pliki_rdowe__FBX
    "lib/FBX/fbxdocument.cpp"
    "lib/FBX/fbxdump.cpp"
    "lib/FBX/fbxnode.cpp"
    "lib/FBX/fbxproperty.cpp"
    "lib/FBX/fbxutil.cpp"
)
source_group("Pliki źródłowe\\FBX" FILES ${Pliki_rdowe__FBX})

set(Pliki_rdowe__UserScripts
    "UserScripts/UserScript.cpp"
)
source_group("Pliki źródłowe\\UserScripts" FILES ${Pliki_rdowe__UserScripts})

set(ALL_FILES
    ${Libraries__ImGui}
    ${Libraries__zlib}
    ${Pliki_nagwkowe}
    ${Pliki_nagwkowe__Cameras}
    ${Pliki_nagwkowe__Editor}
    ${Pliki_nagwkowe__FBX}
    ${Pliki_nagwkowe__UserScripts}
    ${Pliki_zasobw}
    ${Pliki_zasobw__Engine}
    ${Pliki_rdowe}
    ${Pliki_rdowe__Cameras}
    ${Pliki_rdowe__FBX}
    ${Pliki_rdowe__UserScripts}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GLTemplate)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/include;"
        "C:/Users/Minik/Documents/LIB/CPP/Assimp/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/include;"
        "C:/Users/Minik/Documents/LIB/CPP/Assimp/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/include;"
        "C:/Users/Minik/Documents/LIB/CPP/Assimp/include"
    )
endif()

if(NOT GLM_FOUND)
        message(Error "GLM not found")
endif(NOT GLM_FOUND)

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "GLEW_STATIC;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "GLEW_STATIC;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "GLEW_STATIC;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /std:c++17;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /std:c++17;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /std:c++17;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "opengl32;"
        "libglfw;"
        "glfw3dll;"
        "stb_image;"
        "assimp-vc140-mt"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "glfw;"
        "GL"
        "assimp;"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "opengl32;"
        "glfw;"
        "glfw3dll;"
        "stb_image;"
        "glew32s"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/lib/x64;"
        "C:/Users/Minik/Documents/LIB/CPP/Assimp/lib/x64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/lib/x64;"
        "C:/Users/Minik/Documents/LIB/CPP/Assimp/lib/x64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "C:/Users/Minik/Documents/LIB/CPP/OpenGL/lib/x86"
    )
endif()
