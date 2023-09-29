#include "Gui/imgui_ex.hpp"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
// #include "ImGuizmo.h"
namespace ImGui
{

bool BeginViewportSideBarCustom(const char* name, ImGuiViewport* viewport_p, ImGuiDir dir, float axis_size, ImGuiWindowFlags window_flags)
{
    IM_ASSERT(dir != ImGuiDir_None);

    ImGuiWindow* bar_window = ::ImGui::FindWindowByName(name);
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)(viewport_p ? viewport_p : ::ImGui::GetMainViewport());
    if (bar_window == NULL || bar_window->BeginCount == 0)
    {
        // Calculate and set window size/position
        ImRect avail_rect = viewport->GetBuildWorkRect();
        ImGuiAxis axis = (dir == ImGuiDir_Up || dir == ImGuiDir_Down) ? ImGuiAxis_Y : ImGuiAxis_X;
        ImVec2 pos = avail_rect.Min;
        if (dir == ImGuiDir_Right || dir == ImGuiDir_Down)
            pos[axis] = avail_rect.Max[axis] - axis_size;
        ImVec2 size = avail_rect.GetSize();
        size[axis] = axis_size;
        ::ImGui::SetNextWindowPos(pos);
        ::ImGui::SetNextWindowSize(size);
        // Report our size into work area (for next frame) using actual window size
        if (dir == ImGuiDir_Up || dir == ImGuiDir_Left)
            viewport->BuildWorkOffsetMin[axis] += axis_size;
        else if (dir == ImGuiDir_Down || dir == ImGuiDir_Right)
            viewport->BuildWorkOffsetMax[axis] -= axis_size;
    }

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    ::ImGui::SetNextWindowViewport(viewport->ID); // Enforce viewport so we don't create our own viewport when ImGuiConfigFlags_ViewportsNoMerge is set.
    ::ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ::ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0)); // Lift normal size constraint
    bool is_open = ::ImGui::Begin(name, NULL, window_flags);
    ::ImGui::PopStyleVar(2);

    return is_open;
}

bool BeginBigMenuBar()
{
    ImGuiContext& g = *GImGui;
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)::ImGui::GetMainViewport();

    // Notify of viewport change so GetFrameHeight() can be accurate in case of DPI change
    ::ImGui::SetCurrentViewport(NULL, viewport);

    // For the main menu bar, which cannot be moved, we honor g.Style.DisplaySafeAreaPadding to ensure text can be visible on a TV set.
    // FIXME: This could be generalized as an opt-in way to clamp window->DC.CursorStartPos to avoid SafeArea?
    // FIXME: Consider removing support for safe area down the line... it's messy. Nowadays consoles have support for TV calibration in OS settings.
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse;// | ImGuiWindowFlags_MenuBar;
    bool is_open = BeginViewportSideBarCustom("##MainMenuBar", viewport, ImGuiDir_Up, TOOLBAR_HEIGHT, window_flags);
    // g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
    
    // if (is_open){}
    //     // ::ImGui::BeginMenuBar();
    // else
    //     ::ImGui::End();
    return is_open;
}

void EndBigMenuBar()
{
    // ::ImGui::EndMenuBar();

    // When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
    // FIXME: With this strategy we won't be able to restore a NULL focus.
    // ImGuiContext& g = *GImGui;
    // if (g.CurrentWindow == g.NavWindow && g.NavLayer == ImGuiNavLayer_Main && !g.NavAnyRequest)
    //     ::ImGui::FocusTopMostWindowUnderOne(g.NavWindow, NULL);
    ::ImGui::End();
}

bool BeginStatusBar()
{
        ImGuiContext& g = *GImGui;
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)::ImGui::GetMainViewport();

    // Notify of viewport change so GetFrameHeight() can be accurate in case of DPI change
    ::ImGui::SetCurrentViewport(NULL, viewport);

    // For the main menu bar, which cannot be moved, we honor g.Style.DisplaySafeAreaPadding to ensure text can be visible on a TV set.
    // FIXME: This could be generalized as an opt-in way to clamp window->DC.CursorStartPos to avoid SafeArea?
    // FIXME: Consider removing support for safe area down the line... it's messy. Nowadays consoles have support for TV calibration in OS settings.
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ::ImGui::GetFrameHeight(); 
    ::ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    bool is_open = BeginViewportSideBarCustom("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags);
    ::ImGui::PopStyleVar(1);
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);

    
    if (is_open)
        ::ImGui::BeginMenuBar();
    else
        ::ImGui::End();
    return is_open;
}

void EndStatusBar()
{
        ::ImGui::EndMenuBar();

    // When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
    // FIXME: With this strategy we won't be able to restore a NULL focus.
    ImGuiContext& g = *GImGui;
    if (g.CurrentWindow == g.NavWindow && g.NavLayer == ImGuiNavLayer_Main && !g.NavAnyRequest)
        ::ImGui::FocusTopMostWindowUnderOne(g.NavWindow, NULL);
    ::ImGui::End();
}

bool TextImageButtonEx(const char* str_id, ImTextureID texture_id, const ImVec2& s, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
{
    const char* text_end =  ::ImGui::FindRenderedTextEnd(str_id);
    ImVec2 text_size = ::ImGui::CalcTextSize(str_id, text_end, false, 0.0f);
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    unsigned int id = window->GetID(str_id);
    if (window->SkipItems)
        return false;
    ImVec2 size = s;
    ImVec2 bb_custom = s;
    if(s.x < text_size.x)
        bb_custom.x = text_size.x;
    const ImVec2 padding = g.Style.FramePadding; //calculate bounding box before resizing
    
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + bb_custom + padding * 2.0f); 
    float scale = (size.y - text_size.y)/size.y;
    size.x = size.x*scale;
    size.y -= text_size.y;  //subtract Text height;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
    if (bg_col.w > 0.0f)
    window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, GetColorU32(bg_col));
    window->DrawList->AddImage(texture_id, ImVec2(bb.Max.x/2 + bb.Min.x/2 -size.x/2, bb.Min.y), ImVec2(bb.Max.x/2 + bb.Min.x/2 -size.x/2 + size.x, bb.Min.y + size.y), uv0, uv1, GetColorU32(tint_col));

    
    ImVec2 text_pos(bb.Max.x/2 + bb.Min.x/2 - text_size.x/2, bb.Min.y + size.y);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
    ImRect bb_text(text_pos, text_pos + text_size);
    ::ImGui::RenderTextWrapped(bb_text.Min, str_id, text_end, wrap_width);
    return pressed;
}

bool TextImageButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;
    
    return TextImageButtonEx(str_id, user_texture_id, size, uv0, uv1, bg_col, tint_col);
}
}//namespace imgui

namespace ImGuizmo
{

}