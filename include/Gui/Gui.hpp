#ifndef GC_FRONTEND
#define GC_FRONTEND

#include "Program.hpp"
#include <thread>
#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <iostream>
#include "Gui/Console.hpp"
#include <GLFW/glfw3.h>
#include "gleq/gleq.h"
#include "Renderer/Light.hpp"
#include "Renderer/Camera.hpp"
#include "Gui/TextEditor.hpp"
#include "Renderer/SDF.hpp"


class Gui
{
    protected:
        bool m_visible = true;
    public:
        virtual void draw() = 0;
        virtual void handleInput(GLEQevent event) = 0;
};

class MainGui : public Gui
{
    private:
        int m_width = 0, m_height = 0;
        bool show_console = true;
        bool show_texteditor = true;
        bool show_help = false;

        ycad::Camera cam;
        bool is_focused;
        TextEditor text_editor;
        ycad::SDF sdf;

        void showConsole();
        void showTexteditor();
        void showHelp();
        void showViewer();

    public:
        //Callback Varaiables
        // std::function<void(std::filesystem::path p)> callback_file_reader;

        void draw();
        void handleInput(GLEQevent e);

        MainGui();

};

#endif /*GC_FRONTEND*/