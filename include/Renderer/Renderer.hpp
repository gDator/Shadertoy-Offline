////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// This is an 3D extension for SFML
////////////////////////////////////////////////////////////
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Program.hpp"

// #include <oneapi/dpl/algorithm>
// #include <oneapi/dpl/execution>
#include <memory>
#include <fstream>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <tchar.h>
#include <unordered_map>
#include <vector>
#include "logger.hpp"
#include "Basic/Utils.hpp"
#include "Basic/Texture.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/MeshShader.hpp"
#include "Basic/Vector.hpp"
#include "Renderer/Light.hpp"
#include "Renderer/Batch.hpp"
// #include "logger.hpp"

namespace ycad
{



class Renderer
{
    protected:
        GLFWwindow* p_window;
        GLuint m_white_texture;
        std::map<std::string, Shader*> p_shaders;
        Shader* p_active_shader = nullptr;
        Shader* p_standard_shader = nullptr;
        std::map<int, Batch*> a_batches;
        Batch* p_active_batch = nullptr;
        // std::vector<Entity*> p_entities; //Objects and their shader to render
        // std::vector<Light*> p_lights; //Objects and their shader to render
        std::map<std::string, std::vector<std::shared_ptr<ycad::Light>>> p_lights;
        const Vector3f m_color = glm::vec3(0.5f, 0.5f, 1.f);
        int m_width, m_height;
        unsigned int m_fbo;     //TODO: multiple FBOs?
        unsigned int m_texture;
        unsigned int m_depth;
        
        // bool m_size_changed;
        
    public:
        Renderer();
        ~Renderer();
        void init();
        void resize(unsigned int width, unsigned int height);
        void startFrame(Camera &cam); //TODO maybe Sort after ShaderID()?
        void endFrame(); //TODO maybe Sort after ShaderID()?
        void startFrameBuffer(Camera &cam, unsigned int width, unsigned int height);
        unsigned int endFrameBuffer();
        void setWindow(GLFWwindow* window);
        void drawLight(std::shared_ptr<Light> light);

        //Shader Functions
        void addShader(std::string id, Shader* s);
        void activateShader(std::string id);
        void deactivateShader();

        //Draw Functions (batched)
        //Render API 
        void drawCube(Vector3f position, float width, float height, float length, Color color, int mode = GL_QUADS);
        void drawCubeWire(Vector3f position, float width, float height, float length, Color color);
        void drawRay(Ray ray, Color color, float length = 1.f);
        void drawLine(Vector3f a, Vector3f b, Color color);
        void drawQuad(Vector3f pos, Vector3f normal, float width, float height, Color color);
        void drawQuad(Vector3f pos, Vector3f normal, float width, float height, unsigned int texture);
        void drawPoint(Vector3f pos, Color color);
    
        //Internal Render Functions
    private:
        void begin(int mode);
        void end() {} //for consistency
        bool fits(int size);
        void addData(Vertex* vertex, int size);
        void setPoint(Vector3f p);
        void setColor(Color col) {p_active_batch->current_color = col;}
        void setTextureCoord(Vector2f tex) {p_active_batch->current_tex_coords = tex;}
        void setNormal(Vector3f norm) {p_active_batch->current_normal = norm;}
        void setTexture(unsigned int id);
        void disableTexture();

    public:
        struct RenderStats
        {
            int vertecies = 0;
            int batches = 0;
            int DrawCalls = 0;
            float begin_time = 0;
            float end_time = 0;
        };
        RenderStats s_stats;
};

}
#endif /*Renderer*/