#include "Renderer/Camera.hpp"
namespace ycad
{

Matrix4 Camera::update()
{   
    // static glm::mat4 last_view;
    // if(last_view != m_view)
    // {
    //     m_changed_extern = false; //acknowledge change
    //     //calculate backwards if viewmatrix was changed
    //     const float distance = 3.f;
    //     auto viewInverse = glm::inverse(m_view);
        
    //     // m_target = viewInverse[3] - viewInverse[2] * m_settings.focus_distance;
    //     m_position = Vector3f(viewInverse[2][0], viewInverse[2][1], viewInverse[2][2]);
    //     m_up = Vector3f(viewInverse[1][0], viewInverse[1][1], viewInverse[1][2]);
    //     m_target.x = viewInverse[3][0] - m_position.x;
	// 	m_target.y = viewInverse[3][1] - m_position.y;
	// 	m_target.z = viewInverse[3][2] - m_position.z;
    //     // m_position = m_target * distance;
    //     last_view = m_view;
        
    //     // Vector3f s(m_view[0][0], m_view[1][0], m_view[2][0]);
    //     // m_up = glm::cross(s, m_target);
    //     // m_position.x = m_view[3][0];
	// 	// m_position.y = m_view[3][1];
	// 	// m_position.z = m_view[3][2];
    // }
    // else
    // {
        
        w = glm::normalize(m_position - m_target);
        u = glm::normalize(glm::cross(m_up, w));
        v = glm::cross(w, u);
        m_horizontal = m_settings.focus_distance*m_viewport_width * u; 
        m_vertical = m_settings.focus_distance*m_viewport_height * v;
        m_upper_left_corner = m_position - m_horizontal/2.f - m_vertical/2.f - m_settings.focus_distance*w;
        m_view = glm::lookAt(m_position, m_target, m_up);
    // }
    LOG_TRACE(print_point(m_target));
    //projection = glm::perspective(glm::radians(zoom), (float)host->getSize().x/(float)host->getSize().y, 0.1f, 100.0f);^
    // std::cout << "Pos:" << print_point(m_position) << "Target: " << print_point(m_target) << std::endl;
    //  std::cout << "Hrz:" << print_point(u) << "Vert: " << print_point(v) << std::endl;
    
    return m_view;
}

Ray Camera::getRay(float s, float t, float time)
{
    glm::vec3 rd = m_settings.lens_radius * random_in_unit_disk();
    glm::vec3 offset;
    if(m_settings.depth_of_focus)
        offset = u * rd.x + v*rd.y;
    else
        offset = glm::vec3(0,0,0);
    return Ray(m_position + offset, m_upper_left_corner + s*m_horizontal + t*m_vertical - m_position - offset, time);
}
        
Camera::Camera(glm::vec3 pos , glm::vec3 tar, glm::vec3 u, CameraSettings settings)
{
    LOG_TRACE("Camera::Camera");
    setFOV(settings.fov, 16.f/9.f); //assmue 16:9
    
    m_position = pos;
    m_target = tar;//glm::normalize(m_position-tar); //direction vector; not a fixed point
    m_up = u;
    
    m_settings  = settings;
    m_yaw = 0; 
    m_pitch = 0;
    m_roll = 0;
    update();
}

bool Camera::handleKeyboard(GLEQevent e)
{
    switch(e.type)
    {
        //Keyboard active
        case GLEQ_KEY_REPEATED:
        case GLEQ_KEY_PRESSED:
        {
            glm::vec3 move = glm::vec3(0,0,0);
            glm::vec3 dir = glm::normalize(m_target - m_position);
            switch (e.keyboard.key)
            {
                
                case GLFW_KEY_W:
                    m_position += m_settings.movement_speed*dir;
                    break;
                case GLFW_KEY_A:
                    m_position -= glm::normalize(glm::cross(dir, m_up)) * m_settings.movement_speed;
                    break;
                case GLFW_KEY_S:
                    m_position -= m_settings.movement_speed*dir;
                    break;
                case GLFW_KEY_D:
                    m_position += glm::normalize(glm::cross(dir, m_up)) *m_settings.movement_speed;
                    break;
                case GLFW_KEY_TAB:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    break;
            }
            
        }
        break;
        case GLEQ_KEY_RELEASED:
        {
            switch (e.keyboard.key)
            {
                case GLFW_KEY_TAB:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;
            }
            
            break;
        }
        default: return false;
    }
    return true;
}

bool Camera::handleMouse(GLEQevent e)
{
    glm::vec2 mouse_pos(e.pos.x, e.pos.y);
    if(m_mouse_init) //call once to initialize the camera pos
    {
        m_last_x = mouse_pos.x;
        m_last_y = mouse_pos.y;
        m_mouse_init = false; 
    }
    switch (e.type)
    {
        case GLEQ_BUTTON_PRESSED:
        {

        }
        break;
        case GLEQ_BUTTON_RELEASED:
        {
        }
        break;
        case GLEQ_CURSOR_MOVED:
        {
            float x_offset = mouse_pos.x - m_last_x;
            float y_offset =  m_last_y - mouse_pos.y;
            m_last_x = mouse_pos.x;
            
            m_last_y = mouse_pos.y;
            m_yaw += x_offset * m_settings.sensitivity;
            m_pitch += y_offset * m_settings.sensitivity;
            if(m_pitch > 89.0f)
                m_pitch = 89.0f;
            if(m_pitch < -89.0f)
                m_pitch = -89.0f;

            m_target.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_target.y = sin(glm::radians(m_pitch));
            m_target.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
            m_target = 90.f*glm::normalize(m_target); // allow 90 Degress
        }
        break;
        case GLEQ_SCROLLED:
        {
            if(m_settings.zoom >= m_settings.zoom_min && m_settings.zoom <= m_settings.zoom_max)
                m_settings.zoom += e.scroll.y;
            if(m_settings.zoom <= m_settings.zoom_min)
                m_settings.zoom = m_settings.zoom_min;
            if(m_settings.zoom >= m_settings.zoom_max)
                m_settings.zoom = m_settings.zoom_max;
        }
        break;
        default: return false;
    }
    return true;
}

void Camera::settings(CameraSettings settings)
{
    m_settings = settings;
}

bool Camera::handleInput(GLEQevent e)
{
    bool updated = false;
    switch (e.type)
    {
    case GLEQ_KEY_PRESSED:
    case GLEQ_KEY_RELEASED:
        updated = handleKeyboard(e);
        break;
    default:
        updated = handleMouse(e);
        break;
    }
    return updated;
}

void Camera::setFOV(float fov, float aspect_ratio)
{
    m_settings.fov = fov;
    float theta = m_settings.fov/180.f *3.1419;
    float h = tan(theta/2);
    m_viewport_height = 2.0*h;
    m_viewport_width = m_viewport_height*aspect_ratio;
    getProjectionMatrix(aspect_ratio);
    this->update();
}

Matrix4* Camera::getProjectionMatrix(float aspect_ratio)
{
    //TODO: Ortho and Perspective Matrix
    m_projection_matrix = glm::perspective(glm::radians(m_settings.fov), aspect_ratio, m_settings.near_plane, m_settings.far_plane);
    return &m_projection_matrix;
}
}