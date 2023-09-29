#include "Basic/Vector.hpp"
#include <iostream>

Vector3f calculateNormal(Vector3f a, Vector3f b, Vector3f c)
{
    return glm::normalize(glm::cross(b-a, c-a));
}

float calculateAngle(Vector3f a, Vector3f b)
{
    return std::acos(glm::dot(a,b)/(glm::length(a)*glm::length(b)))*180/glm::pi<float>();
}

std::vector<float> toOneDimensionalVector(std::vector<Vector3f> vec)
{
    std::vector<float> r;
    for(auto e : vec)
    {
        r.push_back(e.x);
        r.push_back(e.y);
        r.push_back(e.z);
    }
    return r;
}

std::vector<float> toOneDimensionalVector(std::vector<Vector2f> vec)
{
    std::vector<float> r;
    for(auto e : vec)
    {
        r.push_back(e.x);
        r.push_back(e.y);
    }
        
    return r;
}

std::vector<float> toOneDimensionalVector(std::vector<Vector4f> vec)
{
    std::vector<float> r;
    for(auto e : vec)
    {
        r.push_back(e.x);
        r.push_back(e.y);
        r.push_back(e.z);
        r.push_back(e.w);
    }
        
    return r;
}

Matrix4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, float scale)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, translation);

	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	matrix = glm::scale(matrix, glm::vec3(scale));

	return matrix;
}

// [SECTION] glShader Class
void print_matrix(glm::mat4 a)
{
    std::cout << "|" << a[0].x << " " << a[0].y << " " << a[0].z << " " << a[0].w << "|" << std::endl;
    std::cout << "|" << a[1].x << " " << a[1].y << " " << a[1].z << " " << a[1].w << "|" << std::endl;
    std::cout << "|" << a[2].x << " " << a[2].y << " " << a[2].z << " " << a[2].w << "|" << std::endl;
    std::cout << "|" << a[3].x << " " << a[3].y << " " << a[3].z << " " << a[3].w << "|" << std::endl;
}

std::string print_point (glm::vec3 a)
{
    //std::cout << a.x << "," << a.y << ", " << a.z;
    return "";
}