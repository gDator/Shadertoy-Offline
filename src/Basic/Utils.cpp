#include "Basic/Utils.hpp"

class Material;
struct hit_record;

void process(float percent)
{
	std::cout << "|";
	for(int i = 0; i < int(percent*100); i++)
	{
		std::cout << "=";
	}
	for(int i = 0; i < int((1-percent)*100); i++)
	{
		std::cout << " ";
	}
	std::cout << "|" << percent*100 << "%" << "\r";
}

float clamp(float value, float min, float max)
{
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

float random_float()
{
    static thread_local std::uniform_real_distribution<float> distribution (0, 1.0);
    static thread_local std::mt19937 generator;
    return distribution(generator);
}

float random_float(float min, float max)
{
    return min + (max -min ) *random_float();
}

glm::vec3 random_vec3(float min, float max)
{
    return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

glm::vec3 random_vec3()
{
    return random_vec3(0.f, 1.f);
}

glm::vec3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = random_vec3(-1, 1);
        if(glm::length(p)*glm::length(p) >= 1) continue;
        return p;
    }
}

glm::vec3 random_unit_vector()
{
    return glm::normalize(random_in_unit_sphere());
}

glm::vec3 random_in_hemisphere(glm::vec3 &normal)
{
    glm::vec3 in_unit_sphere = random_in_unit_sphere();
    if(glm::dot(in_unit_sphere, normal) > 0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

bool near_zero(glm::vec3 vec)
{
    float eps = 1e-8;
    return fabs(vec.x) < eps && fabs(vec.y) < eps && fabs(vec.z) < eps;
}

glm::vec3 random_in_unit_disk()
{
    while(true)
    {
        auto p = glm::vec3(random_float(-1, 1), random_float(-1, 1), 0);
        if(glm::length(p)*glm::length(p) >= 1) continue;
        return p;
    }
}

void split(const std::string& s, char delim, std::vector<std::string>& elems)
{
	std::stringstream ss(s);
	
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
}

int interpolate_colorbar(int max, std::vector<Vector4f> &colors, std::vector<std::vector<int>> ref)
{
    double r, g, b;
    int mod = max%(ref.size()-2);
    int j = (max + ref.size()-2 -mod)/(ref.size()-2);
    //std::cout << mod << ", " << j << ", ";
    for(int i = 0; i < ref.size() -1; i++)
    {
        //für jede Farbe n zwischenfarben einfügen
        for(int k = 0; k < j; k++)
        {   
            double percentage = static_cast<double>(k)/j;
            //std::cout <<percentage<< ", ";
            
            std::vector<int> c1 = ref[i];
            std::vector<int> c2 = ref[i+1];
            if(c2[0] > c1[0]) // red
                r = c1[0] + (c2[0] - c1[0])*(percentage); //prozent innerhalb der zwei farben ist die nachkommastelle
            else
                r = c1[0] + (c2[0] - c1[0])*(percentage);
            if(c2[1] > c1[1])   //green
                g = c1[1] + (c2[1] - c1[1])*(percentage); //prozent innerhalb der zwei farben ist die nachkommastelle
            else
                g = c1[1] + (c2[1] - c1[1])*(percentage);
            if(c2[2] > c1[2])   //blue
                b = c1[2] + (c2[2] - c1[2])*(percentage); //prozent innerhalb der zwei farben ist die nachkommastelle
            else
                b = c1[2] + (c2[2] - c1[2])*(percentage);
            //std::cout << r << ", " << g << ", " << b << std::endl;
            colors.push_back(Vector4f(r,g,b, 1));
        }
    }
    return colors.size();
}

