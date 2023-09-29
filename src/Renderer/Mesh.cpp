#include "Renderer/Mesh.hpp"

#include "logger.hpp"
#include "GLFW/glfw3.h"
#include "Basic/Utils.hpp"
#include "stb/stb_image.h"
#include <fstream>
#include "Geometry/Geometry.hpp"

namespace ycad
{

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices, bool calc_normals) : m_vertices(vertices), m_indices(indices)
{
    createVAO();
    unbindVAO();
    updatePoints(calc_normals);
}
Mesh::Mesh()
{
    createVAO();
    unbindVAO();
}

Mesh::~Mesh()
{
    LOG_TRACE("");
    glDeleteVertexArrays(1, &m_vao_id);
    for(auto vbo : m_vbo_ids)
    glDeleteBuffers(1, &vbo);
}


void Mesh::clearData()
{
    if(m_vbo_ids.size() > 0)
        for(auto vbo : m_vbo_ids)
            glDeleteBuffers(1, &vbo);

}

int Mesh::pointExists(glm::vec3 p)
{
    int exists = -1;
    for(int i = 0; i < m_vertices.size(); i++)
    {
        if(m_vertices[i].position == p)
            exists = i;
    }
    return exists;
}

glm::vec3& getPoint(int i)
{

}

void Mesh::addPoint(Vector3f point, Vector4f color, Vector2f texcoord )
{
    addPoint(false, false, point, color, texcoord);
}

void Mesh::newSet()
{
    LOG_TRACE("newSet()");
    m_internal_count = 0;
}

void Mesh::addTriangle(std::vector<Vector3f> points, std::vector<Vector4f> colors, std::vector<Vector2f> texcoords)
{
    addPoint(points[0], colors[0], texcoords[0]);
    addPoint(points[1], colors[1], texcoords[1]);
    addPoint(points[2], colors[2], texcoords[2]);
}

void Mesh::addQuad(std::vector<Vector3f> points, std::vector<Vector4f> colors, std::vector<Vector2f> texcoords)
{
    LOG_TRACE("Meshl::AddQuad\n");
    addPoint(points[0], colors[0], texcoords[0]);
    addPoint(points[1], colors[1], texcoords[1]);
    addPoint(points[2], colors[2], texcoords[2]);
    addPoint(points[3], colors[3], texcoords[3]);
}

void Mesh::addPoint(bool update_on_call, bool duplicate_point, Vector3f point, Vector4f color, Vector2f texcoord)
{
    static unsigned int last_added, before_last_added;    
    bool exists = false;
    int index = pointExists(point);
    if(index >= 0 && !duplicate_point) //check if vertex already exists 
    {
        LOG_WARN("Point Exists");
        exists = true;
        point = m_vertices[index].position;
    }

    if(m_internal_count >= 3)
    {
        glm::vec3 normal = glm::normalize(calculateNormal(point, m_vertices[last_added].position, m_vertices[before_last_added].position));
        if(normal.y > 0)
        {
            m_indices.push_back(last_added);
            m_indices.push_back(before_last_added);
        }
        else
        {
            m_indices.push_back(before_last_added);
            m_indices.push_back(last_added);
        }
        LOG_TRACE("Connect: " << m_indices[m_indices.size() - 2] << "," << m_indices[m_indices.size() - 1] << "," << m_vertex_count);
    }

    before_last_added = last_added;
    if(!exists)
    {
        Vertex v;
        v.position = point;
        v.color = color;
        v.tex_coords = texcoord;
        last_added = m_vertex_count;
        m_indices.push_back(m_vertex_count);
        m_vertices.push_back(v);
    }
    else
    {
        last_added = index;
        m_indices.push_back(index);
    }   
    
        
    // if(color.w < 1) //TODO: implement this somehow in Entity
    //     this->setTransparency(true);
    if(update_on_call)
    {
        updatePoints();
    }
    if(!exists)
    {
        m_vertex_count++;
    }
    m_internal_count++;
}

void Mesh::updatePoints(bool calculate_normals)
{
    clearData(); //FIX: prevents multiple vbos without usage
    LOG_TRACE("Mesh::updatePoints");
    if(calculate_normals)
    {
        for(int i = 0; i < m_indices.size() - 2; i += 3)
        {  
            glm::vec3 v1 = m_vertices[m_indices[i]].position, v2 = m_vertices[m_indices[i + 1]].position, v3 = m_vertices[m_indices[i + 2]].position;
            glm::vec3 normal = glm::normalize(calculateNormal(v1, v2, v3));
            // std::cout << m_indices[i] << "(" << print_point(v1)<< ")," << m_indices[i+1] << "(" << print_point(v2) << ")," << m_indices[i+2] << "(" << print_point(v3)<< ")," <<":" << print_point(normal) << std::endl;
            if(glm::all(glm::isnan(normal)));
            m_vertices[m_indices[i]].normal = normal;
            m_vertices[m_indices[i + 1]].normal = normal;
            m_vertices[m_indices[i + 2]].normal = normal;
        }
    }
    
    LOG_TRACE("VAO Size" << m_vertices.size());
    glBindVertexArray(m_vao_id);
    bindIndicesBuffer(m_indices);
    //see Batch::Batch()

    unsigned int vbo_id; 
    glCreateBuffers(1, &vbo_id); //XXX: if adding points and updating them, every time a new buffer gets created and old ones dont get destroyed -> memory leak
    m_vbo_ids.push_back(vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(m_vao_id, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0 * sizeof(GLfloat))); // (GLvoid*)(6 * sizeof(GLfloat)
    glEnableVertexArrayAttrib(m_vao_id, 1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexArrayAttrib(m_vao_id, 2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexArrayAttrib(m_vao_id, 3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat))); 
    
    unbindVAO();
}

void Mesh::storeDataInAttributeList(unsigned int index, unsigned int vector_size, std::vector<float> &data)
{
    LOG_TRACE("storeDataInAttributeList");
    unsigned int vbo_id;
	int size = data.size() * sizeof(data[0]);
    glGenBuffers(1, &vbo_id);
    m_vbo_ids.push_back(vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(index, vector_size, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::bindIndicesBuffer(std::vector<int> &indices)
{
    LOG_TRACE("Mesh::bindInidicesBuffer");
    unsigned int vbo_id;
	int size = indices.size() * sizeof(indices[0]);
	glGenBuffers(1, &vbo_id);
	m_vbo_ids.push_back(vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); dont do that : see learnopengl p.50
}

void Mesh::createVAO()
{
    LOG_TRACE("Mesh::createVAO");
    if(m_vao_id == 0)
	    glGenVertexArrays(1, &m_vao_id);
	glBindVertexArray(m_vao_id);
}

void Mesh::unbindVAO()
{
    LOG_TRACE("unbindVAO");
    glBindVertexArray(0);
}

bool Mesh::hitTriangle(const Ray &r, float t_min, float t_max, HitRecord &rec) const
{

}

bool Mesh::hit(const Ray &r, float t_min, float t_max, HitRecord &rec, glm::mat4 transformation) const
{
    float t_actual = t_max;
    for(int i = 0; i < m_indices.size() - 2; i += 3)
    {
        glm::vec3 v1 = transformation*glm::vec4(m_vertices[m_indices[i]].position,1.f), 
                    v2 = transformation*glm::vec4(m_vertices[m_indices[i + 1]].position, 1.f), 
                    v3 = transformation*glm::vec4(m_vertices[m_indices[i + 2]].position, 1.f);
        glm::vec3 normal = calculateNormal(v1, v2, v3);
        if(glm::dot(normal, r.getDirection()) == 0) //parallel
            continue;
        float t = - glm::dot(normal, r.getOrigin() - v1)/glm::dot(normal, r.getDirection());

        if(t > t_min && t < t_max && t < t_actual)
        { 
            
            glm::vec3 s = r.at(t);
            glm::vec3 b = v2 - v1;
            glm::vec3 c = v3 - v1;
            float length = glm::length(glm::cross(b,c))*glm::length(glm::cross(b,c));
            float beta = (glm::dot(s - v1, b) * glm::length(c) * glm::length(c) -(glm::dot(b,c)*glm::dot(s-v1, c)))/length;
            float gamma = (glm::dot(s - v1, c) * glm::length(b) * glm::length(b) -(glm::dot(b,c)*glm::dot(s-v1, b)))/length;
            if(beta >= 0 && gamma >= 0 && gamma + beta <= 1 && beta <= 1 && gamma <= 1)
            {
                t_actual = t;
                rec.t = t;
                rec.point = s;
                rec.normal = normal;
                // return true;
            }
        }
    }
    if(t_actual != t_max)
        return true;
    else
        return false;
}

std::tuple<glm::vec3, glm::vec3> Mesh::calculateBoundingBox(glm::mat4 transformation)
{
    if(m_indices.size() <= 1) //single point has no bounding box
        return std::make_tuple(glm::vec3(0), glm::vec3(0));
    glm::vec3 min = transformation*Vector4f(m_vertices[m_indices[0]].position, 1);
    glm::vec3 max = transformation*Vector4f(m_vertices[m_indices[0]].position,1);
    for(int i = 1; i < m_indices.size(); i++)
    {
        glm::vec3 v1 = transformation*glm::vec4(m_vertices[m_indices[i]].position,1.f);
        min.x = std::min(min.x, v1.x);
        min.y = std::min(min.y, v1.y);
        min.z = std::min(min.z, v1.z);
        max.x = std::max(max.x, v1.x);
        max.y = std::max(max.y, v1.y);
        max.z = std::max(max.z, v1.z);
    }
    return std::make_tuple(min, max);
}
}//namespace ycad