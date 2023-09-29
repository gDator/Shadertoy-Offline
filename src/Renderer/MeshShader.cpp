#include "Renderer/MeshShader.hpp"
#include "logger.hpp"
#include "Renderer/Light.hpp"

namespace ycad
{
void MeshShader::bindAttributes()
{
    LOG_TRACE("bindAttributes");
	// Associate each 'in' variable with the VAO indexes.
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
    this->bindAttribute(3, "color");
}

void MeshShader::getAllUniformLocations()
{
    LOG_TRACE("getAllUniformLocations");
	// Get all the ids for all the unfiforms for the current vertex and fragment shaders.
	m_transformation_matrix_id = this->getUniformLocation("transformationMatrix");
	m_projection_matrix_id = this->getUniformLocation("projectionMatrix");
	m_view_matrix_id = this->getUniformLocation("viewMatrix");
	m_light_position_id = this->getUniformLocation("lightPosition");
	m_light_color_id = this->getUniformLocation("lightColour");
	m_shine_damper_id = this->getUniformLocation("shineDamper");
	m_reflectivity_id = this->getUniformLocation("reflectivity");

}

void MeshShader::loadTransformationMatrix(glm::mat4 matrix)
{
    LOG_TRACE("loadTransformationMatrix");
	this->loadMatrix(m_transformation_matrix_id, matrix);
}

void MeshShader::loadProjectionMatrix(glm::mat4 projection)
{
    LOG_TRACE("loadProjectionMatrix");
	this->loadMatrix(m_projection_matrix_id, projection);
}

void MeshShader::loadViewMatrix(Matrix4 mat)
{
	LOG_TRACE("readloadViewMatrix");
    this->loadMatrix(m_view_matrix_id, mat);
}

void MeshShader::loadLight(Light* light)
{
    LOG_TRACE("loadLight");
	this->loadVector(m_light_position_id, light->getPosition());
	this->loadVector(m_light_color_id, light->getColor());
}

void MeshShader::loadShineVariables(float damper, float reflectivity)
{
    LOG_TRACE("loadShineVariables");
	this->loadFloat(m_shine_damper_id, damper);
	this->loadFloat(m_reflectivity_id, reflectivity);
}

} //namespace xad