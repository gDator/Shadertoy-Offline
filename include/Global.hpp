#ifndef GLOBAL_VAR_HPP
#define GLOBAL_VAR_HPP
#include "Renderer/Renderer.hpp"
#include "Gui/Console.hpp"
#include "Basic/TextureAtlas.hpp"
#include "Program.hpp"


inline ycad::Renderer g_renderer;
inline bool console_open;
inline Console g_console("Console", &console_open);
inline TextureAtlas g_icons;

inline xgl::program::Program app;
#endif /*GLOBAL_HPP*/