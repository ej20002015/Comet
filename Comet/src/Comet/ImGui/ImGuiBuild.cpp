#include "CometPCH.h"

//COULD HAVE BEEN INCLUDED VIA PREMAKE BUT WANTED TO KEEP ALL COMET SPECIFIC IMGUI LOGIC TOGETHER IN THIS DIRECTORY

//To specifiy the OpenGL loader Comet is using
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"