#include "Renderer/SDF.hpp"
#include <GL/glew.h>
#include <iostream>
namespace ycad
{
SDFShader::SDFShader(const std::string &vert, const std::string &frag) : Shader(vert, frag, 0)
{
}

SDFShader::~SDFShader()
{
}

void SDFShader::bindAttributes()
{
	this->bindAttribute(0, "position");
}

void SDFShader::getAllUniformLocations() 
{
	m_mouse_pos_id = this->getUniformLocation("iMousePos");
	m_time = this->getUniformLocation("iTime");
    m_resolution_id = this->getUniformLocation("iResolution");
}

void SDFShader::loadTime(float time)
{
    if(m_time != -1)
    loadFloat(m_time, time);
}

void SDFShader::loadMousePos(glm::vec2 pos)
{
    if(m_mouse_pos_id != -1)
        loadVector(m_mouse_pos_id, pos);
}

void SDFShader::loadResolution(glm::vec2 vec)
{
    if(m_resolution_id != -1)
        loadVector(m_resolution_id, vec);
}

SDF::SDF(int width, int height) : m_width(width), m_height(height)
{
    std::string s; 
    for(auto& line: fragment_shader_input)
        s+= line;
    p_shader = std::make_shared<SDFShader>(vertex_shader, std::string(fragment_shader_base + s));
    p_shader->getAllUniformLocations();
    glCreateVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);
    glBindVertexArray(vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SDF::~SDF()
{
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
}

void SDF::updateFragmentShader(std::vector<std::string> shader) 
{
    fragment_shader_input = shader;
    std::string s; 
    for(auto& line: fragment_shader_input)
        s+= line;
    p_shader = std::make_shared<SDFShader>(vertex_shader, std::string(fragment_shader_base + s));
    p_shader->getAllUniformLocations();
}

void SDF::update(int width, int height)
{
    if(width == m_width && height == m_height)
        return;
    m_width = width;
    m_height = height;
}

void SDF::begin(float time, Vector2f mouse_pos)
{
    static float t = 0;
    t+= 1./60;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    p_shader->use();
    p_shader->loadTime(t);
    p_shader->loadResolution(Vector2f(m_width, m_height));
    p_shader->loadMousePos(mouse_pos);
}

void SDF::end()
{
    
    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    p_shader->dontUse();
}

}//namespace ycad

