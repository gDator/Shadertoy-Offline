#include "Renderer/LineShader.hpp"
#include "logger.hpp"

namespace ycad
{
void LineShader::bindAttributes()
{
    LOG_TRACE("bindAttributes");
	// Associate each 'in' variable with the VAO indexes.
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
    this->bindAttribute(3, "color");
}

void LineShader::getAllUniformLocations()
{
    LOG_TRACE("getAllUniformLocations");
	// Get all the ids for all the unfiforms for the current vertex and fragment shaders.
	m_projection_matrix_id = this->getUniformLocation("projectionMatrix");
	m_view_matrix_id = this->getUniformLocation("viewMatrix");
	m_transformation_matrix_id = this->getUniformLocation("transformationMatrix");
}

void LineShader::loadProjectionMatrix(glm::mat4 projection)
{
    LOG_TRACE("loadProjectionMatrix");
	this->loadMatrix(m_projection_matrix_id, projection);
}

void LineShader::loadViewMatrix(Matrix4 mat)
{
	LOG_TRACE("readloadViewMatrix");
    this->loadMatrix(m_view_matrix_id, mat);
}







} //namespace xad