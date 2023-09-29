#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Basic/Vector.hpp"
#include "Renderer/MeshShader.hpp"
namespace ycad
{

//TODO::DirectionalLight; add SpotLight etc?
class Light
{
    public: 
        virtual void draw(Shader& s) = 0;
        virtual inline glm::vec3 getPosition() = 0;
        virtual inline glm::vec3 getColor() = 0;
};

class DirectionalLight : public Light
{
    private:
        Vector3f m_position;
        Vector3f m_color;
    public:
        inline glm::vec3 getPosition() {return m_position;}
        inline glm::vec3 getColor() {return m_color;}
        inline void setPosition(glm::vec3 pos) {m_position = pos;}
        inline void setColor(glm::vec3 col) {m_color = col;}
        void draw(Shader& s) {s.loadLight(this);}
        DirectionalLight() {}
        DirectionalLight(glm::vec3 pos, glm::vec3 col = glm::vec3(1,1,1)) : m_position(pos), m_color(col) {}
        ~DirectionalLight() {}
};
} 
#endif /*LIGHT_HPP*/