#ifndef GUI_STATS_HPP
#define GUI_STATS_HPP

#include "Gui/Gui.hpp"
class GuiStats : public Gui
{
    protected:
        bool m_visible;
    public:
        virtual void draw();
        virtual void handleInput(GLEQevent event);
};

#endif /*GUI_STATS*/