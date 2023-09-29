#include "Gui/Console.hpp"


void Console::linkVisibleVaraibale(bool* v)
{
    p_open = v;
}   

void Console::addCommand(std::string cmd, void* fun, std::string hlp)
{
    commands.push_back(new Command(cmd, fun, this, hlp));
}
void Console::removeFunction(std::string c)
{
}

void Console::parse()
{
    if(!command_line.empty())
        printLine(std::string ("$ " + command_line + "\n"));
    else
        return;
        
    if (command_line.compare("clear") == 0)
    {
        clearLog();
    }
    else if (command_line.compare("help") == 0)
    {
        help();
    }
    else if (command_line.compare("h") == 0 || command_line.compare("history") == 0)
    {
        int first = history.size() - 10;
        for (int i = first > 0 ? first : 0; i < history.size(); i++)
            printLine(("\t%i: %s\n", i, history[i]));
    }
    else
    {
        std::string c = "";
        std::vector<std::string> arguments = {};
        for(auto cmd : commands)
        {
            if(cmd->getCommand().compare(c) == 0)
            {
                //execute(arguments);
                return;
            }
        }
        throwError(std::string ("Unknown command:" + command_line + "\n"));
    }
}

void Console::help()
{
    for(auto cmd : commands)
    {
        printLine("\t" + cmd->getHelp());
        printLine("\n");
    }
}

void Console::info()
{
    printLine("#XGL CONSOLE CLASS\n alpha v1.0");
}

void Console::clearLog()
{
    log.clear();
}

void Console::addHistory(std::string h)
{
    history.push_back(h);
}

void Console::draw()
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title.c_str() , p_open))
    {
        ImGui::End();
        return;
    }

    // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
    // So e.g. IsItemHovered() will return true when hovering the title bar.
    // Here we create a context menu only available from the title bar.
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Close Console"))
            *p_open = false;
        ImGui::EndPopup();
    }

    // TODO: display items starting from the bottom
    //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); printLine("Spam %f", t); }

    ImGui::Separator();

    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &autoscroll);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) clearLog();
        ImGui::EndPopup();
    }

    // Display every line as a separate entry so we can change their color or add custom widgets.
    // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
    // to only process visible items. The clipper will automatically measure the height of your first item and then
    // "seek" to display only items in the visible area.
    // To use the clipper we can replace your standard loop:
    //      for (int i = 0; i < Items.Size; i++)
    //   With:
    //      ImGuiListClipper clipper;
    //      clipper.Begin(Items.Size);
    //      while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // - That your items are evenly spaced (same height)
    // - That you have cheap random access to your elements (you can access them given their index,
    //   without processing all the ones before)
    // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
    // We would need random-access on the post-filtered list.
    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
    // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
    // to improve this example code!
    // If your items are of variable height:
    // - Split them into same height items would be simpler and facilitate random-seeking into your list.
    // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    bool copy_to_clipboard = ImGui::SmallButton("Copy");
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    
    for (int i = 0; i < log.size(); i++)
    {
        const char* item = log[i].c_str();
        if (!filter.PassFilter(item))
            continue;

        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]"))          { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
        else if (strncmp(item, "#", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
        else if (strstr(item, "[warning]")) { color = ImVec4(255, 255, 149, 255); has_color = true; }
        else if (strstr(item, "[info]")) { color = ImVec4(0, 162 ,232 ,255); has_color = true; }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }
    
    if (copy_to_clipboard)
        ImGui::LogFinish();

    if (scroll_to_buttom || (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    scroll_to_buttom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", &command_line, input_text_flags, [] (ImGuiInputTextCallbackData* data) -> 
                                int {Console* console = (Console*)data->UserData; return console->TextEditCallback(data);}))
    {
        parse();
        command_line = "";
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void Console::printLine(std::string l)
{
    log.push_back(l);
}

void Console::throwError(std::string l)
{
    printLine("[error]" + l);
}

void Console::throwWarning(std::string l)
{
    printLine("[warning]" + l);
}

void Console::throwInfo(std::string l)
{
    printLine("[info]" + l);
}

std::string Command::getHelp()
{
    return help;
}

std::string Command::getCommand()
{
    return command;
}

void* Command::getCallBackFunction()
{
    return function;
}

std::string Console::execute(std::vector<std::string> args)
{

    // Insert into history. First find match and delete it so it can be pushed to the back.
    // This isn't trying to be smart or optimal.
    history_idx = -1;
    for (int i = history.size() - 1; i >= 0; i--)
        if (history[i].compare(command_line) == 0)
        {
            history[i];
            history.erase(history.begin() + i);
            break;
        }
    history.push_back(command_line);

    // Process command
    // On command input, we scroll to bottom even if AutoScroll==false
    scroll_to_buttom = true;
}

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
    //first do resize if nessecary
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* s = (std::string*)data->UserData;
        IM_ASSERT(s->begin().base() == data->Buf);
        // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
        //Note:: do not use BufSize, but BufTextLen; BufSize adds a free space, which generates random signs
        s->resize(data->BufTextLen); 
        data->Buf = s->begin().base();
    }

    //printLine("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            std::vector<std::string> candidates;
            for (int i = 0; i < commands.size(); i++)
                if (commands[i]->getCommand().compare(word_start) == 0)
                    candidates.push_back(commands[i]->getCommand());

            if (candidates.size() == 0)
            {
                // No match
                printLine(("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start));
            }
            else if (candidates.size() == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0].c_str());
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0].c_str(), candidates[0].c_str() + match_len);
                }

                // List matches
                printLine("Possible matches:\n");
                for (int i = 0; i < candidates.size(); i++)
                    printLine(("- %s\n", candidates[i]));
            }

            break;
        }
    case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = history_idx;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (history_idx == -1)
                    history_idx = history.size() - 1;
                else if (history_idx > 0)
                    history_idx--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (history_idx != -1)
                    if (++history_idx >= history.size())
                        history_idx = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != history_idx)
            {
                const char* history_str = (history_idx >= 0) ? history[history_idx].c_str() : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
    }
    return 0;
}