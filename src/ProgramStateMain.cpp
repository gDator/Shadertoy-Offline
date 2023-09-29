#include "ProgramStateMain.hpp"
#include "Gui/GuiStats.hpp"
#include "imgui_spectrum.h"
#include "Global.hpp"

namespace xgl
{
    namespace program
    {
        /*Konstruktor of Task state*/
        ProgramStateTest::ProgramStateTest()                                        
        {
            LOG_TRACE("");
            console_open = true;
            // ycad::physics::PhysicSpace simulator;
            //Add icons
            loadIcons();
            g_renderer.init();

            //Add Guis
            gui.push_back(std::make_shared<MainGui>());
            gui.push_back(std::make_shared<GuiStats>());
            ImGui::Spectrum::LoadFont(16.f);
            ImGui::Spectrum::StyleColorsSpectrumDark();
        }

        /*
            @name: draw
            @func: MainLoop of the Game; manage all the incoming states
            @args: none
            @return: none
        */
        void ProgramStateTest::draw(const float dt, GLFWwindow* window)
        {
            
            //DEB(lg, trace) << "ProgramStateTask::draw()";
            //program->getRenderWindow().clear(xgl::graphics::getXglImguiColors(xgl::graphics::WindowBg));
            for(auto& g: gui)
                g->draw();
            //ImGui::ShowDemoWindow();
            // ImPlot::ShowDemoWindow();
            // renderer.render(cam, false);
        }

        /*
            @name: update
            @func: 
            @args: @dt is the time between two update processes
            @return: none
        */
        void ProgramStateTest::update(const float dt)
        {
            //DEB(lg, trace) << "ProgramStateTask::update()";
        }
        

        bool ProcessEvent(GLEQevent& event)
        {
            if(!ImGui::GetIO().WantCaptureMouse)
                return false;
            ImGuiIO& io = ImGui::GetIO();
            switch (event.type)
            {
                case GLEQ_SCROLLED:
                {
                    if (event.scroll.x > 0) io.MouseWheelH += 1;
                    if (event.scroll.x < 0) io.MouseWheelH -= 1;
                    if (event.scroll.y > 0) io.MouseWheel += 1;
                    if (event.scroll.y < 0) io.MouseWheel -= 1;
                    return true;
                }
                case GLEQ_BUTTON_PRESSED:
                {
                    if (event.mouse.button == 0) io.MouseDown[0] = true;
                    if (event.mouse.button == 1) io.MouseDown[1] = true;
                    if (event.mouse.button == 2) io.MouseDown[2] = true;
                    return true;
                }
                case GLEQ_CODEPOINT_INPUT:
                {
                    //io.AddInputCharactersUTF8(event->codepoint);
                    io.AddInputCharacter(event.codepoint);
                    return true;
                }
                case GLEQ_KEY_PRESSED:
                case GLEQ_KEY_RELEASED:
                {
                    //std::cout << (event->keyboard.mods) << std::endl;
                    int key = event.keyboard.key;
                    IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
                    io.KeysDown[key] = (event.type == GLEQ_KEY_PRESSED);
                    io.KeyShift = (event.keyboard.mods == 1);
                    io.KeyCtrl = (event.keyboard.mods == 2);
                    io.KeyAlt = (event.keyboard.mods == 4);
                    io.KeySuper = (event.keyboard.mods == 8);
                    return true;
                }
            }
            return false;
        }
        /*
            @name: handleInput
            @func: Handle Inputs in Main Menu; Event can be:
                    @Window closed
                    @Window resized
                    @key pressed

            @args: none
            @return: none
        */
        bool ProgramStateTest::handleInput(GLFWwindow* window)
        {
            GLEQevent event;
            bool event_on_stack = false;
            while(gleqNextEvent(&event))
            {
                event_on_stack = true;
                // ProcessEvent(event); in Program.cpp callbacks already installed for imgui
                for(auto& g : gui)
                    g->handleInput(event);
                gleqFreeEvent(&event);
            }
            return event_on_stack;
            //DEB(lg, trace) << "ProgramStateTask::handleInput()";
            //Process all Events    
        }

        /*
            @name: handleKeyboard
            @func: Process all Keyboard Inputs
            @args: @event
            @return: none
        */
        void ProgramStateTest::handleKeyboard()
        {
            //DEB(lg, trace) << "ProgramStateTask::handleKeyboard()";
        }

        /*
            @name: handleMouse
            @func: Process all Mouse Inputs
            @args: @event
            @return: none
        */
        void ProgramStateTest::handleMouse()
        {
            //DEB(lg, trace) << "ProgramStateTask::handleMouse()";
        }

        /*
            @name: loadIcons
            @func: load Icons From Resource Folder
            @args: @event
            @return: none
        */
        void ProgramStateTest::loadIcons()
        {
        }
    }
}