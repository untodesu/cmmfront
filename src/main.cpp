#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <globals.hpp>
#include <gui.hpp>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

static void on_glfw_error(int code, const char *str)
{
    std::cerr << str << std::endl;
    std::terminate();
}

int main(int argc, char **argv)
{
    int width, height;

    glfwSetErrorCallback(&on_glfw_error);

    if(!glfwInit()) {
        std::cerr << "glfwInit() failed" << std::endl;
        std::terminate();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);

    globals::window = glfwCreateWindow(800, 600, "cmmfront", nullptr, nullptr);
    if(!globals::window) {
        std::cerr << "glfwCreateWindow() failed" << std::endl;
        std::terminate();
    }

    glfwMakeContextCurrent(globals::window);
    glfwSwapInterval(1);

    if(!gladLoadGL(&glfwGetProcAddress)) {
        std::cerr << "gladLoadGL() failed" << std::endl;
        std::terminate();
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigDockingWithShift = false;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(globals::window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    while(!glfwWindowShouldClose(globals::window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwGetFramebufferSize(globals::window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gui::draw(width, height);

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(globals::window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(globals::window);
    glfwTerminate();

    return 0;
}
