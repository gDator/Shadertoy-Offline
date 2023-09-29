#ifndef IMGUI_EX_HPP
#define IMGUI_EX_HPP

#include "imgui.h"

#define TOOLBAR_HEIGHT 100
namespace ImGui
{
    bool BeginViewportSideBarCustom(const char* name, ImGuiViewport* viewport_p, ImGuiDir dir, float axis_size, ImGuiWindowFlags window_flags);
    bool BeginBigMenuBar();
    void EndBigMenuBar();

    bool BeginStatusBar();
    void EndStatusBar();
    bool TextImageButtonEx(const char* str_id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
    bool TextImageButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

}

namespace ImGuizmo
{
    
}



#endif /*IMGUI_EX_HPP*/