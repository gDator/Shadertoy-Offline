#ifndef LINE_SHADER_HPP
#define LINE_SHADER_HPP

#include "Renderer/Shader.hpp"

namespace ycad
{

class LineShader : public Shader
{
    private:
        unsigned int m_projection_matrix_id; 
        unsigned int m_view_matrix_id;
        unsigned int m_transformation_matrix_id;
    public:
        LineShader(const std::string &vert, const std::string &frag, std::string geometry = "") : Shader(vert, frag, geometry) {}
        ~LineShader() {}
        void bindAttributes();
        void getAllUniformLocations();
        void loadTransformationMatrix(glm::mat4 matrix) {this->loadMatrix(m_transformation_matrix_id, glm::mat4 (1.0f));}
        void loadProjectionMatrix(glm::mat4 projection);
        void loadViewMatrix(Matrix4 camera);
        void loadLight(Light* light) {}
        void loadShineVariables(float damper, float reflectivity) {}
        void loadClearColor(glm::vec3 value) {}
        void loadTextureColorMixture(float mixture) {}
};
} //namespace ycad
#endif /*MESH_SHADER_HPP*/