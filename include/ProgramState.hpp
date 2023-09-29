#ifndef PROGRAMM_STATE_HPP
#define PROGRAMM_STATE_HPP

#include "SFML/Graphics.hpp"
#include "Program.hpp"
namespace xgl
{
namespace program
{
    class ProgramState
    {
        public:
            virtual void draw(const float dt, GLFWwindow* window) = 0;
            virtual void update(const float dt) = 0;
            virtual bool handleInput(GLFWwindow* window) = 0;
            virtual void handleKeyboard() = 0;
            virtual void handleMouse() = 0;
    };
}
}

#endif /*PROGRAMM_STATE*/