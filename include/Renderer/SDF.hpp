#ifndef SDF_HPP
#define SDF_HPP

#include "Renderer/Shader.hpp"
#include <string>

namespace ycad
{
class SDFShader : public Shader
{
    private:
        int m_time;
        int m_mouse_pos_id;
        int m_resolution_id;
    public:
        SDFShader(const std::string &vert, const std::string &frag);
        ~SDFShader();
        void bindAttributes() override;
        void getAllUniformLocations() override;
        void loadTime(float time) override;
        void loadMousePos(glm::vec2 pos) override;
        void loadResolution(glm::vec2 vec) override; //only sdf
        
};


class SDF
{
    private:
   
        std::shared_ptr<SDFShader> p_shader;
        unsigned int vao_id = 0, vbo_id = 0;
        mutable int m_width = 0, m_height = 0;
        Vector3f buffer[6] = {Vector3f(1, 1, 0.0f), Vector3f(-1,  1, 0.0f), Vector3f(1, -1, 0.0f), Vector3f(-1, 1, 0), Vector3f(-1, -1, 0), Vector3f(1, -1, 0)};
        const std::string vertex_shader =  "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position, 1.0);\n"
        "}\0";

        std::string fragment_shader_base = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform float iTime;\n"
        "uniform vec2 iResolution;\n"
        "uniform vec2 iMousePos;\n"
        "vec2 coord = gl_FragCoord.xy;\n";
        
          //main will be edited through user
        std::vector<std::string> fragment_shader_input = {"void main()", "{", "vec2 uv = coord/iResolution;", 
        "vec3 col = vec3(0.5f) + 0.5*cos(vec3(iTime)+vec3(uv, 0)+vec3(0,2,4));","color = vec4(col, 1.0f);", "}"};
        
    public:
        SDF(int width, int height);
        ~SDF();
        void update(int width, int height);
        void begin(float time, Vector2f mouse_pos);
        void end();
        void updateFragmentShader(std::vector<std::string> s);
        std::vector<std::string>& getFragmentShader() {return fragment_shader_input;}

};
} //namespace ycad
#endif /*SDF_HPP*/