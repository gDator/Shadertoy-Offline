#include <stack>
#include "ProgramState.hpp"
#include "Program.hpp"
#include "logger.hpp"
#include <iostream>
#include "stb_image.h"
#include "Global.hpp"

void cmsg(std::string msg)
{   
    g_console.printLine(msg);
}

void cmsg(const char* msg)
{   
    g_console.printLine(msg);
}

namespace xgl
{
namespace program
{

Program::Program()
{
    //initLog("log", 1); //only log file
    if (!glfwInit())
        LOG_ERROR("GLFW not initialized");
        //DEB(lg, trace) << "Program::Program()";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    gleqInit();
    window = glfwCreateWindow(720, 720, "ycad", NULL, NULL);
    g_renderer.setWindow(window);
    int channels;
    image.width = 32;
    image.height = 32;
    image.pixels = stbi_load("res/icon.png", &image.width, &image.height, &channels, 0);
    glfwSetWindowIcon(window, 1, &image);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gleqTrackWindow(window);

    bool glewExperimental = true;  
    if(!glewInit())
        assert("GLEW not initialized");
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.Fonts->AddFontFromFileTTF("res/UbuntuMono.ttf", 13.0);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

Program::~Program()
{
    while(!this->states.empty())
        popState();
}


/*
    @name: pushState
    @func: Add a new GameState
    @args: @state which should ne added 
    @return: none
*/
void Program::pushState(ProgramState* state)
{
    //DEB(lg, trace) << "Program::pushState()";
    this->states.push(state);
    return;
}


/*
    @name: popState
    @func: remove the topmost GameState
    @args: none
    @return: none
*/

void Program::popState()
{
    //DEB(lg, trace) << "Program::popState";
    //delete this->states.top();
    this->states.pop();

    return;
}

/*
    @name: changeState
    @func: Changing the State
    @args: @state is the state to which should be changed 
    @return: none
*/

void Program::changeState(ProgramState* state)
{
    //DEB(lg, trace) << "Program::changeState()";
    if(!this->states.empty())
        popState();
    pushState(state);
    return;
}

/*
    @name: peekState
    @func: Looking at the next state to work with
    @args: none
    @return: the first @GameState
*/

ProgramState* Program::peekState()
{
    //DEB(lg, trace) << "Program::peekState()";
    if(this->states.empty())
        return nullptr;
    return this->states.top();
}

/*
    @name: loop
    @func: MainLoop of the Game; manage all the incoming states
    @args: none
    @return: none
*/
void Program::loop()
{
    //DEB(lg, trace) << "Program::loop()";
    ImGuiIO& io = ImGui::GetIO();
    float dt = 1e-3;
    float last_dt = 0;
    bool redraw_gui = true;
    bool first_frame, second_frame = true;
    
    while(!glfwWindowShouldClose(window))
    {
        
        if(peekState() == nullptr)
        {
            std::cout<< "error: no state\n"; 
            break;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        redraw_gui = peekState() -> handleInput(window);
        // if(redraw_gui)
        // {
            peekState() -> update(dt);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame(); 
            ImGui::ShowDemoWindow(NULL);
            peekState() -> draw(dt, window); 
            ImGui::Render();
        // }
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
       


        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            // if(redraw_gui)
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        dt = glfwGetTime();
        if(dt != 0)
        {
            std::cout << "FPS: " << (int)1/(dt-last_dt) << "\r";
            last_dt = dt;
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

/*
    @name: init
    @func: initializerfunction
    @args: none
    @return: none
*/
void Program::init()
{

}

GLFWwindow* Program::getRenderWindow()
{
    return window;
} 


}
}
