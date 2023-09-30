#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "Basic/Vector.hpp"

namespace ycad
{

class Light;
class Shader
{
    private:
        unsigned int m_vertex_id;
        unsigned int m_fragment_id;
        int m_geometry_id = -1;
    protected:
        unsigned int m_shader_id;
        std::string loadShader(const std::string &filename);
        unsigned int createShader(std::string shader, unsigned int type);
    public:
        virtual void bindAttributes() = 0;
        virtual void getAllUniformLocations() = 0;
        virtual void loadProjectionMatrix(glm::mat4 projection) {};
        virtual void loadViewMatrix(glm::mat4 view) {};
        virtual void loadLight(Light* l) {}; //only for draw Light
        virtual void loadShineVariables(float s, float r) {}; //only for draw entity
        virtual void loadTransformationMatrix(glm::mat4 transformation) {}; //only for draw entity
        virtual void loadTime(float time) {} //only sdf
        virtual void loadResolution(glm::vec2 vec) {} //only sdf
        virtual void loadMousePos(glm::vec3 pos) {}  //only sdf
        virtual void loadFrameCount(int frame) {}  //only sdf
        
        void bindAttribute(unsigned int attribute, std::string variable_name);
        unsigned int getUniformLocation(const std::string &uniform_name);
        void loadInt(unsigned int location, int value);
        void loadFloat(unsigned int location, float value);
        void loadVector(unsigned int loaction, Vector3f vector);
        void loadVector(unsigned int location, Vector2f vector);
        void loadBool(unsigned int location, bool value);
        void loadMatrix(unsigned int location, Matrix4 matrix);
        void use();
        void dontUse();
        unsigned int getShaderID();
        Shader(const std::string &vertex_path, const std::string &fragment_path, std::string geometry_path = "");
        Shader(const std::string &vertex_shader, const std::string &fragment_shader, int pseudeo_input, std::string geometry_shader = "");
        ~Shader();
};
} //namespace ycad

#endif /*SHADER_HPP*/