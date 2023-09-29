#include "Tools/MeshCreator.hpp"

#if 0 //not needed for Shader toy
Mesh MeshCreator::fromFile(std::string file)
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::read_triangle_mesh(file, V, F);
    return fromEigenMatrix(V, F);
}

Mesh MeshCreator::fromEigenMatrix(Eigen::MatrixXd& V, Eigen::MatrixXi& F)
{
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    for(auto row : V.rowwise())
    {
        Vertex v;
        int i = 0;
        for(auto vertex : row)
        {
            v.position[i] = (float)vertex; 
            i++;
        }
        v.tex_coords = Vector2f(0.f, 0.f);
        v.color = Color(1.f, 0.0f, 0.0f, 1.f);
        vertices.push_back(v);        
    } 
    
    for(auto row : F.rowwise())
        for(auto face : row)
            indices.push_back((int)face);
    return Mesh(vertices, indices, true);
}

Mesh MeshCreator::fromEigenMatrix(Eigen::MatrixXd V, Eigen::MatrixXi F , Eigen::MatrixXd TC, Eigen::MatrixXd N)
{
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    for(auto row : V.rowwise())
    {
        Vertex v;
        int i = 0;
        for(auto vertex : row)
        {
            v.position[i] = vertex; 
            i++;
        }
        v.color = Color(1.f, 1.f, 1.f, 1.f);
        vertices.push_back(v);
    }
    for(auto row : F.rowwise())
        for(auto face : row)
            indices.push_back((int)face);

    int j = 0;
    for(auto row : TC.rowwise())
    {
        Vector3f tex;
        int i = 0;
        for(auto tc : row)
        {
            tex[i] = tc; 
            i++;
        } 
        assert(j < vertices.size());
        vertices.at(j).tex_coords = tex; 
        j++;
    }
    j=0;
    for(auto row : N.rowwise())
    {
        Vector3f normal;
        int i = 0;
        for(auto n : row)
        {
            normal[i] = n; 
            i++;
        } 
        assert(j < vertices.size());
        vertices.at(j).normal = normal; 
        j++;
    }          
    
    return Mesh(vertices, indices, true);
}

#endif