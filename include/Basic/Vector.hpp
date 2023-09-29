#ifndef VECTOR_HPP
#define VECTOR_HPP


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
//[SECTION] Typedefs
typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;
typedef glm::vec<2, int, glm::defaultp> Vector2i;
typedef glm::vec<3, int, glm::defaultp> Vector3i;
typedef glm::vec<4, int, glm::defaultp> Vector4i;

typedef  glm::vec4 Color;
typedef  glm::mat4 Matrix4;

//Vector Util Functions
std::vector<float> toOneDimensionalVector(std::vector<Vector3f> vec);
std::vector<float> toOneDimensionalVector(std::vector<Vector2f> vec);
std::vector<float> toOneDimensionalVector(std::vector<Vector4f> vec);
Vector3f calculateNormal(Vector3f a, Vector3f b, Vector3f c);
float calculateAngle(Vector3f a, Vector3f b);
std::string print_point (glm::vec3 a);

//Matrix Util functions
Matrix4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, float scale);
void print_matrix(glm::mat4 a);

#endif /*Geometry*/