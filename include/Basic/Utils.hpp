#ifndef UTIL_HPP
#define UTIL_HPP

#include "Basic/Vector.hpp"


#include <sstream>
#include <iostream>
#include <random>

inline std::vector<std::vector<int>> cool = {{0, 255, 255},{0, 125, 255},{200, 100, 255},{255, 255, 255}, {255, 50, 200},{255, 0, 200},{100, 0, 200},{0,0,0}};

class Material;
struct hit_record;

void process(float percent);

float clamp(float value, float min, float max);

float random_float();

float random_float(float min, float max);

glm::vec3 random_vec3();

glm::vec3 random_in_unit_sphere();

glm::vec3 random_unit_vector();

glm::vec3 random_in_hemisphere(glm::vec3 &normal);

bool near_zero(glm::vec3 vec);

glm::vec3 random_in_unit_disk();

void split(const std::string& s, char delim, std::vector<std::string>& elems);

int interpolate_colorbar(int max, std::vector<Vector4f> &colors, std::vector<std::vector<int>> ref);



#endif /*RT_HPP*/