#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <memory>
#include <string>
#include "Basic/Vector.hpp"
#include "Renderer/Ray.hpp"

#define POINT_SELECTION_SIZE 0.1
#define LINE_SELECTION_SIZE 0.1

#define YCAD_ECS_GEOMETRY std::shared_ptr<ycad::Geometry>

namespace ycad
{
class Material;
class Geometry;

struct HitRecord
{
    public:
        Vector3f point;
        Vector3f normal;
        double t;
        bool front_face;
        std::shared_ptr<Material> material;
        const Geometry* object; //TODO: record the hit object
        inline void set_face_normal(const Ray &r, const glm::vec3 &outward_normal)
        {
            front_face = glm::dot(r.getDirection(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

enum class GeometryType {POINT, LINE, AABB, LAYER, PLANE, SKETCH};
class Geometry
{
    protected:
        std::vector<Geometry*> p_update_list;
    public:
        virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const = 0;
        virtual void update() = 0;
        virtual void appendUpdateList(Geometry* obj) {p_update_list.push_back(obj);}
};

} //namespace ycad

#endif /*GEOMETRY_HPP*/