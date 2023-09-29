#ifndef PROGRAM_STATE_TEST_HPP
#define PROGRAM_STATE_TEST_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"
#include "ProgramState.hpp"
#include "Gui/Console.hpp"
#include "Gui/Gui.hpp"
#include <functional>
#include <mutex>
#include "Renderer/Renderer.hpp"
#include "Renderer/LineShader.hpp"
#define DEBUG
#include "logger.hpp"



namespace xgl
{
    namespace program
    {
        class ProgramStateTest : public ProgramState
        {
            private: 
                int error;
                std::vector<std::shared_ptr<Gui>> gui;
                void loadIcons();
                
            public:
                unsigned char* pixels;
                int width, height;
                std::string database;
                         
                void draw(const float dt, GLFWwindow* window);
                void update(const float dt);
                bool handleInput(GLFWwindow* window);
                void handleKeyboard();
                void handleMouse();

                ProgramStateTest();
        };
    }
}

#endif /*PROGRAMM_STATE_TEST_HPP*/