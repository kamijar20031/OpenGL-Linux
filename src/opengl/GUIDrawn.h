#ifndef GUIIIII_H_
#define GUIIIII_H_

#include "physics.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class GuiModule
{
    public:
    GuiModule() {};
    GuiModule(GLFWwindow* window);
    void draw(PhysicsModule* physics);
};

#endif