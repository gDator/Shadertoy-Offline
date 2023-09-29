#include "Gui/GuiStats.hpp"
#include "Global.hpp"

void GuiStats::draw()
{
    if(ImGui::Begin("Stats"))
    {
        ImGui::Text("Render Stats");
        ImGui::NewLine();
        ImGui::Text("Vertecies: %i", g_renderer.s_stats.vertecies);
        ImGui::NewLine();
        ImGui::Text("Batches: %i", g_renderer.s_stats.batches);
        ImGui::NewLine();
        ImGui::Text("Draw Calls: %i", g_renderer.s_stats.DrawCalls);
        ImGui::NewLine();
        float time_passed = g_renderer.s_stats.end_time - g_renderer.s_stats.begin_time;
        ImGui::Text("Render Time [ms]: %.3f", time_passed*1000);
        ImGui::NewLine();
        ImGui::Text("FPS: %.1f", 1/time_passed);


    }
    ImGui::End();
}

void GuiStats::handleInput(GLEQevent event)
{

}
