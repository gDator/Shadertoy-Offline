#include "Program.hpp"
#include "Gui/Gui.hpp"
#include "FileDialog.hpp"
#include "Gui/imgui_ex.hpp"
#include "Global.hpp"


MainGui::MainGui() :cam(glm::vec3(-10.f, 10.f, -10.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)), sdf(m_width, m_height)
{
    g_console.linkVisibleVaraibale(&show_console);
    text_editor.SetTextLines(sdf.getFragmentShader());
    text_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
}

void MainGui::draw()
{
    // static ImGuiTabBarFlags tab_bar_flags
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Load File"))
            {
                std::string path = FileDialog::OpenFile("Shader (*.vert, *.frag, *glsl)\0");
                if(path != "")
                {
                    std::vector<std::string> file;
                    std::ifstream shader(path);
                    while(!shader.eof())
                    {
                        std::string buffer;
                        std::getline(shader, buffer);
                        file.push_back(buffer);
                    }
                    shader.close();
                    text_editor.SetTextLines(file);
                }
            }
            if(ImGui::MenuItem("Save File"))
            {
                std::string path = FileDialog::SaveFile("Shader (*.vert, *.frag, *glsl)\0");
                if(path != "")
                {
                    std::vector<std::string> file = text_editor.GetTextLines();
                    std::ofstream shader(path);

                    for(auto& l : file) 
                        shader << l << std::endl;
                    shader.close();
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit"))
        {
            if(ImGui::MenuItem("Run")) {sdf.updateFragmentShader(text_editor.GetTextLines());}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Viewer", NULL, &m_visible);
            ImGui::MenuItem("Console", NULL, &show_console);
            ImGui::MenuItem("Texteditor", NULL, &show_texteditor);
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("How to use")) {show_help = true;}
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if(show_console)
        showConsole();
    if(show_texteditor)
        showTexteditor();
    if(show_help)
        showHelp();
    if(m_visible)
        showViewer();
}

void MainGui::showViewer()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f,0.f)); //NO padding on the view window
    if(ImGui::Begin("View", &m_visible, ImGuiWindowFlags_NoCollapse |ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImVec2 win_pos = ImGui::GetWindowPos();
        if(ImGui::IsWindowFocused())
            is_focused = true;
        else
            is_focused = false;
        
        //io.MousePos
        Vector2f size = ImGui::GetCurrentContext()->CurrentWindow->SizeFull;
    
        static ImVec2 uv_min = ImVec2(0.0f, 1.0f);                 // Top-left
        static ImVec2 uv_max = ImVec2(1.0f, 0.0f);                 // Lower-right
        static ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        static ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        m_width = size.x - 2* uv_max.x;
        m_height = size.y - 2* uv_min.y;
        cam.setFOV(cam.m_settings.fov, (float)m_width/m_height);
        // GLFWimage view;
        // view.width = 300;
        // view.height = 300;
        // view.pixels = renderer.drawToFrameBuffer(cam, 300, 300);
        g_renderer.startFrameBuffer(cam, m_width, m_height);
        ImGuiIO& io = ImGui::GetIO();
        Vector2f pos = Vector2f(io.MousePos)-Vector2f(win_pos);
        sdf.begin(glfwGetTime(), pos);
        sdf.update(m_width, m_height);
        sdf.end();
        //raytracer
        //ImGui::Image((ImTextureID)m_ray.getID(), m_ray.getSize(), uv_min, uv_max, tint_col, border_col);
        ImGui::Image((ImTextureID)g_renderer.endFrameBuffer(), ImVec2(m_width, m_height), uv_min, uv_max, tint_col, border_col);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void MainGui::showConsole()
{
    g_console.draw();
}

void MainGui::handleInput(GLEQevent e)
{
    if(is_focused)
        if(cam.handleInput(e))
        {
            cam.update();
        }
}

void MainGui::showTexteditor()
{
    auto cpos = text_editor.GetCursorPosition();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_MenuBar;
    if(ImGui::Begin("Texteditor", &show_texteditor, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {   
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    auto textToSave = text_editor.GetText();
                    /// save text....
                }
                if (ImGui::MenuItem("Quit", "Alt-F4"))
                    show_texteditor = false;
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edit"))
            {
                bool ro = text_editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    text_editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && text_editor.CanUndo()))
                    text_editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && text_editor.CanRedo()))
                    text_editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, text_editor.HasSelection()))
                    text_editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && text_editor.HasSelection()))
                    text_editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor.HasSelection()))
                    text_editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                    text_editor.Paste();

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    text_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(text_editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Dark palette"))
                    text_editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    text_editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    text_editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
           ImGui::EndMenuBar();
        }
        
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, text_editor.GetTotalLines(),
        text_editor.IsOverwrite() ? "Ovr" : "Ins",
        text_editor.CanUndo() ? "*" : " ",
        text_editor.GetLanguageDefinition().mName.c_str(), "fileToEdit");
        text_editor.Render("TextEditor");
    }
    ImGui::End();
}

void MainGui::showHelp()
{
    if(ImGui::Begin("How To", &show_help, ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::Text("Similiar to Shader Toy;");
        ImGui::Text("Shader has following inputs/outputs");
        ImGui::Bullet();
        ImGui::Text("iMousePos (vec2) This is the current mouse position inside the view window");
        ImGui::Bullet();
        ImGui::Text("iResolution (vec2): This is the current viewer window size");
        ImGui::Bullet();    
        ImGui::Text("iTime (float): Passed time since startup");
        ImGui::Bullet();
        ImGui::Text("coord (vec2): Is the coord of the current fragment");
        ImGui::Bullet();
        ImGui::Text("color (vec4): Is the color of the current fragment. Needs to be set");
        ImGui::Text("Load Files *.glsl, *.vert, *.frag from filesystem with File->Load File");
        ImGui::Text("Write current Shader to file filesystem with File-> Save File");
        ImGui::Text("If any question write me an mail: infos.g.infos@gmail.com");
        ImGui::Text("Edit Shader in Texteditor and Press Edit->Run to compile Shader.\n If any Errors occur Console will show errors");
            
    }
    ImGui::End();
}

