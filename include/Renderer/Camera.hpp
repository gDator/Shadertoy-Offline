#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Basic/Vector.hpp"

#include "gleq/gleq.h"
#include "Renderer/Ray.hpp"
#include "logger.hpp"
#include <iostream>
#include "Basic/Utils.hpp"
namespace ycad
{
struct CameraSettings
{
    float zoom = 45, zoom_max = 1, zoom_min = 100;
    float movement_speed = 0.5f; //moving
    float sensitivity = 1;

    float lens_radius = 0.1;
    float focus_distance = 1;
    bool depth_of_focus = false;
    float fov = 45;
    float near_plane = 0.1;
    float far_plane = 1000;
}; 


class Camera
{   
    private:
        glm::vec3 m_position;
        glm::vec3 u, v, w;        
        glm::mat4 m_view;
        glm::vec3 m_target;
        glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 1.0f);
        glm::vec3 m_horizontal;
        glm::vec3 m_vertical;
        Matrix4 m_projection_matrix; 
        glm::vec3 m_upper_left_corner;        
        float m_yaw, m_pitch, m_roll;
        float m_last_x, m_last_y;
        bool m_mouse_init = true;        
        float m_viewport_height;
        float m_viewport_width;
        bool m_changed_extern;
        

        //overwrite these functionms if you want to change the handling
        bool handleKeyboard(GLEQevent e);
        bool handleMouse(GLEQevent e);
    public:    

        CameraSettings m_settings;
        inline void move(Vector3f delta);// { m_position += delta;} //? TODO glm::normalize(glm::cross(m_target, m_up))*glm::abs(delta)
        inline void increaseYaw(int theta) {m_yaw += theta;}
        inline void increasePitch(int theta) {m_pitch += theta;}
        inline void increaseRoll(int theta) {m_roll += theta;}
        // inline Vector3f getPosition() {return m_position;}
        inline float getYaw() {return m_yaw;}
        inline float getPitch() {return m_roll;}
        inline float getZoom() {return m_settings.zoom;}
        inline const glm::mat4* getViewMatrix() {return &m_view;}
        inline glm::mat4* getViewMatrixEditable() {m_changed_extern = true; return &m_view;}
        inline glm::vec3* getTarget() {return &m_target;}
        inline glm::vec3* getPosition() {return &m_position;}
        inline glm::vec3* getUp() {return &m_up;}
        // inline void setViewportSize(int width, int height) {m_viewport_width = width; m_viewport_height = height; update();}
        void settings(CameraSettings settings);
        bool handleInput(GLEQevent e);
        Matrix4 update();
        Ray getRay(float s, float t, float time = 0);
        Matrix4* getProjectionMatrix(float aspect_ratio);
        void setFOV(float fov, float aspect_ratio);
        Camera(Vector3f pos = Vector3f(400.0f, 1.0f, 3.0f), Vector3f tar = Vector3f(0.0f, 0.0f, -1.0f), Vector3f u = Vector3f(0.0f, 1.0f, 0.0f), CameraSettings settings = CameraSettings());
};
}

#endif /*CAMERA_HPP*/