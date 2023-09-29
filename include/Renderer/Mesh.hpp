#ifndef Mesh_HPP
#define Mesh_HPP

#include "Basic/Vertex.hpp"
#include "GL/glew.h"
#include "Renderer/Ray.hpp"
#include "Geometry/Geometry.hpp"

namespace ycad 
{

class Mesh
{
    private:
    protected:
        unsigned int m_vao_id = 0;
        unsigned int m_vertex_count = 0;
        unsigned int m_internal_count = 0;
        std::vector<unsigned int> m_vbo_ids;
        std::vector<int> m_indices;
        std::vector<Vertex> m_vertices;
        
        void createVAO();
        void unbindVAO();
        int pointExists(glm::vec3 p); 
        void bindIndicesBuffer(std::vector<int> &indices);
        void storeDataInAttributeList(unsigned int index, unsigned int vector_size, std::vector<float> &data);
    public: 
        Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<int> indices, bool calc_normals = false);
        ~Mesh();
               
        unsigned int getIndexCount() const {return m_indices.size();}
        unsigned int getVaoID() const {return m_vao_id;}
        void clearData();
        void addPoint(Vector3f point, Vector4f color = glm::vec4(1, 1, 1, 1), Vector2f texcoord = glm::vec2(0,0));
        void addPoint(bool update_on_call, bool duplicate_point, Vector3f point, Vector4f color = glm::vec4(1, 1, 1, 1), Vector2f texcoord = glm::vec2(0,0));
        void addTriangle(std::vector<Vector3f> points, 
                        std::vector<Vector4f> colors = {glm::vec4(1, 1, 1, 1),glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1)}, 
                        std::vector<Vector2f> texcoords = {glm::vec2(0,0), glm::vec2(0,0),glm::vec2(0,0)});
        void addQuad(std::vector<Vector3f> points, 
                        std::vector<Vector4f> colors = {glm::vec4(1, 1, 1, 1),glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1)}, 
                        std::vector<Vector2f> texcoords = {glm::vec2(0,0), glm::vec2(0,0),glm::vec2(0,0), glm::vec2(0,0)});
        void updatePoints(bool calculate_normalse = false);
        void newSet();
        bool hitTriangle(const Ray &r, float t_min, float t_max, HitRecord &rec) const;
        bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec, glm::mat4 transformation) const;
        std::tuple<glm::vec3, glm::vec3> calculateBoundingBox(glm::mat4 transformation);
};
}
#endif /*Mesh_HPP*/