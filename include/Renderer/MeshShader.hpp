#ifndef MESH_SHADER_HPP
#define MESH_SHADER_HPP

#include "Renderer/Shader.hpp"

namespace ycad
{

class MeshShader : public Shader
{
    private:
        unsigned int m_transformation_matrix_id;
        unsigned int m_projection_matrix_id; 
        unsigned int m_view_matrix_id;
        unsigned int m_light_position_id;
        unsigned int m_light_color_id;
        unsigned int m_shine_damper_id;
        unsigned int m_reflectivity_id;
    public:
        MeshShader(const std::string &vert, const std::string &frag, std::string geometry = "") : Shader(vert, frag, geometry) 
        {
        }
        ~MeshShader() {}
        void bindAttributes() override;
        void getAllUniformLocations() override;
        void loadTransformationMatrix(glm::mat4 matrix) override;
        void loadProjectionMatrix(glm::mat4 projection) override;
        void loadViewMatrix(Matrix4 camera) override;
        void loadLight(Light* light) override;
        void loadShineVariables(float damper, float reflectivity) override;
};
} //namespace ycad
#endif /*MESH_SHADER_HPP*/