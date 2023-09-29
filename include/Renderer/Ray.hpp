#ifndef RAY_HPP
#define RAY_HPP

#include "Basic/Vector.hpp"

namespace ycad
{
class Ray
{
    private:
        glm::vec3 m_origin;
        glm::vec3 m_direction;
        float m_time;
    public:
        Ray() {}
        Ray(const glm::vec3 &origin, const glm::vec3 &dir, float time = 0.f) : m_origin(origin), m_direction(dir), m_time(time) {}
        ~Ray() {}
        inline glm::vec3 at(float t) const {return m_origin + m_direction*t;}
        glm::vec3 getOrigin() const {return m_origin;}
        glm::vec3 getDirection() const {return m_direction;}
        float getTime() const {return m_time;}
};
}

#endif //RAY_HPP