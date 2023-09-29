#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include <boost/circular_buffer.hpp>


class Console;
class Command
{
    private: 
        std::string command;
        std::string help;
        void* function;
        Console* parent; //define for calling from command and printing e.g. processbars
    public:
        std::string getHelp();
        std::string getCommand();
        void* getCallBackFunction();
        Command(std::string cmd, void* fun, Console* con , std::string hlp = "")
        {
            parent = con;
            command = cmd;
            help = hlp;
            function = fun;
        }
        
};

class Console
{
    private:
        std::vector<Command*> commands;
        boost::circular_buffer<std::string> log;
        std::vector<std::string> history  = {};
        bool* p_open;
        std::string title;
        int history_idx;
        bool autoscroll;
        bool scroll_to_buttom;
        ImGuiTextFilter filter;
        std::string command_line;

    public:
        
        void addCommand(std::string cmd, void* fun, std::string hlp = "");
        void removeFunction(std::string c);
        void parse();
        void help();
        void info();
        void draw();
        void printLine(std::string l);
        void throwError(std::string l);
        void throwWarning(std::string l);
        void throwInfo(std::string l);
        void clearLog();
        void addHistory(std::string h);
        std::string execute(std::vector<std::string> args);
        void linkVisibleVaraibale(bool* v);
        int TextEditCallback(ImGuiInputTextCallbackData* data);
        Console(std::string t, bool* p_op, int buffer_size = 1000)
        {
            title = t;
            p_open = p_op;
            history_idx = -1;
            autoscroll = true;
            scroll_to_buttom = true;
            log.resize(buffer_size);
        }
            
};

#endif /*CONSOLE HPP*/