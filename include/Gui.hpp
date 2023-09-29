#ifndef GC_FRONTEND
#define GC_FRONTEND

#include <thread>
#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <iostream>
#include "Gui/Console.hpp"
#include <GLFW/glfw3.h>


class Gui
{
    private:
        bool show_console = false;
        bool show_file_explorer = false;
        bool show_search = false;
        bool fullscreen = false;
        
        std::filesystem::path selected_path;
        Console* console;

        void showConsole(GLFWwindow* w);
        void showFileExplorer(GLFWwindow *w);
        void showSearch(GLFWwindow* w);

    public:
        //Callback Varaiables
        // std::function<void(std::filesystem::path p)> callback_file_reader;

        void draw(GLFWwindow* w);
        
        Gui(Console* c)
        {
            console = c;
            console->linkVisibleVaraibale(&show_console);
            ImGui::GetCurrentContext()->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        }

};

#endif /*GC_FRONTEND*/