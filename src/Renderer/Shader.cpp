#include "Renderer/Shader.hpp"

#include "GL/glew.h"
#include "logger.hpp"
#include <fstream>

#include "Global.hpp"
namespace ycad
{

unsigned int Shader::createShader(std::string shader, unsigned int type)
{
    unsigned int shader_id(glCreateShader(type));
    const char* source = shader.c_str();
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);
    int success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        g_console.throwError(std::string("ERROR::SHADER::VERTEX::COMPILATION::FAILED\n") + std::string(infoLog));
    }

    return shader_id;
}

std::string Shader::loadShader(const std::string &filename)
{
    LOG_TRACE("loadShader");
    std::string shader_code;
    std::ifstream shader_file;
    LOG_TRACE("Read Shader: " + filename);
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //open files
        shader_file.open(filename);
        std::stringstream shader_stream;

        //Read file buffer contents into stream;

        shader_stream << shader_file.rdbuf();

        //close stream
        shader_file.close();

        //convert stream to GLchar
       shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    return shader_code;
}

Shader::Shader(const std::string &vertex_shader, const std::string &fragment_shader, int pseudeo_input, std::string geometry_shader)
{
    m_vertex_id = createShader(vertex_shader, GL_VERTEX_SHADER);
    m_fragment_id = createShader(fragment_shader, GL_FRAGMENT_SHADER);
        //Compile shaders
    m_shader_id = glCreateProgram();
    glAttachShader(m_shader_id, m_vertex_id);
    glAttachShader(m_shader_id, m_fragment_id);
    if(geometry_shader != "")
    {
        m_geometry_id = createShader(geometry_shader, GL_GEOMETRY_SHADER);
        glAttachShader(m_shader_id, (unsigned int) m_geometry_id);
    }
    glLinkProgram(m_shader_id);
    
    int success;
    char infoLog[512];
    glGetProgramiv(m_shader_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_shader_id, 512, NULL, infoLog);
        g_console.throwError(std::string("ERROR::PROGRAM::LINK::FAILED\n") + std::string(infoLog));
    }
    glValidateProgram(m_shader_id);
}


Shader::Shader(const std::string &vertex_path, const std::string &fragment_path, std::string geometry_path) : Shader(loadShader(vertex_path),loadShader(fragment_path),0)
{
}

Shader::~Shader()
{   
    LOG_TRACE("~Shader");
    glDeleteShader(m_vertex_id);
    glDeleteShader(m_fragment_id);
    glDeleteProgram(m_shader_id);
    if(m_geometry_id != -1)
    {
       glDeleteProgram(m_geometry_id);
    }
}

void Shader::use()
{
    glUseProgram(m_shader_id);
    bindAttributes();
	//getAllUniformLocations();
}

void Shader::dontUse()
{
    glUseProgram(0);
}

unsigned int Shader::getShaderID()
{
    return m_shader_id;
}

void Shader::bindAttribute(unsigned int attribute, std::string variable_name)
{
    glBindAttribLocation(m_shader_id, attribute, variable_name.c_str());
}

unsigned int Shader::getUniformLocation(const std::string &uniform_name)
{
    return glGetUniformLocation(m_shader_id, uniform_name.c_str());
}

void Shader::loadFloat(unsigned int location, float value)
{
    glUniform1f(location, value);
}

void Shader::loadVector(unsigned int location, Vector3f vector)
{
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::loadVector(unsigned int location, Vector2f vector)
{
    glUniform2f(location, vector.x, vector.y);
}

void Shader::loadBool(unsigned int location, bool value)
{
    if(value)
        glUniform1f(location, GL_TRUE);
    else
        glUniform1f(location, GL_FALSE);
}

void Shader::loadMatrix(unsigned int location, Matrix4 matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

} //namespace ycad