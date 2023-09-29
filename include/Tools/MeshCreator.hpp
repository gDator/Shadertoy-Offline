#ifndef MESH_CREATOR_HPP
#define MESH_CREATOR_HPP

#if 0 // not needed for this program
#include "Renderer/Mesh.hpp"
#include <Eigen/Dense>

namespace ycad
{
class MeshCreator
{
    private:
        MeshCreator();
    public:
        static Mesh fromFile(std::string path);
        static Mesh fromEigenMatrix(Eigen::MatrixXd V, Eigen::MatrixXi F);
        static Mesh fromEigenMatrix(Eigen::MatrixXd V, Eigen::MatrixXi F , Eigen::MatrixXd TC, Eigen::MatrixXd N);

};
}
#endif

#endif /*MESH_LOADER*/