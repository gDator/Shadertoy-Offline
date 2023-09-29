#include "Renderer/Renderer.hpp"
#define DEBUG
#include "logger.hpp"
#include "stb_image.h"
namespace ycad
{


Renderer::Renderer()
{
    //Do nothing here; GL wont be initialized
}

Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_texture);

    for(auto&& [id, batch] : a_batches)
        delete batch;
}

void Renderer::init()
{
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    glGenTextures(1, &m_texture);
    glGenTextures(1, &m_depth);
    glGenFramebuffers(1, &m_fbo);
    glPointSize(10.f);
    // glEnable(GL_PROGRAM_POINT_SIZE);
}

void Renderer::begin(int mode)
{
    if(a_batches.count(mode) == 0)  //batch for this mode does not exist
    {
        auto batch = new Batch(mode);
        a_batches.emplace(mode, batch);
        //stats
        s_stats.batches++;
    }
    p_active_batch = a_batches.at(mode);
    assert(p_active_batch->p_active_draw_call->vertex_count% p_active_batch->p_active_draw_call->vertex_alignment ==0);
}
    
bool Renderer::fits(int size)
{
    return true;
}

void Renderer::disableTexture()
{
    p_active_batch->disableTexture();
    //activate white texture
    // p_active_batch->current_tex_index = 0;
}

void Renderer::setTexture(unsigned int id)
{
    assert(p_active_batch != nullptr);
    s_stats.DrawCalls += p_active_batch->setTexture(id);
}

void Renderer::setPoint(Vector3f p)
{
    assert(p_active_batch != nullptr);
    s_stats.DrawCalls += p_active_batch->setPoint(p);   //could be a flush
    //stats
    s_stats.vertecies++;
}

void Renderer::activateShader(std::string id)
{
    assert(p_shaders.count(id) > 0);
    p_active_shader = p_shaders.at(id);
}

void Renderer::deactivateShader()
{
    p_active_shader = p_standard_shader;
}

void Renderer::startFrame(Camera &cam)
{
    LOG_TRACE("render");
    assert(p_window != nullptr);
    // assert(p_standard_shader != nullptr);
    glEnable(GL_DEPTH_TEST);
	// Clear the screen with a default colour.
	
    glClearColor(m_color.x, m_color.y, m_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //activate shader
    if(p_active_shader == nullptr) //activate standdard_shader if none is activated
        p_active_shader = p_standard_shader;
    if(p_active_shader != nullptr) //shader must be set extern!
    {
        p_active_shader->use();
        p_active_shader->loadViewMatrix(cam.update());
        p_active_shader->loadProjectionMatrix(*cam.getProjectionMatrix((float)m_width/(float)m_height));
        glBindTexture(GL_TEXTURE_2D, m_white_texture);
    }
    

    //Render Stats
    s_stats.begin_time = glfwGetTime();

    //reset stats
    s_stats.vertecies = 0;
    s_stats.DrawCalls = 0; //reset after reading
}

void Renderer::endFrame()
{
    if(p_active_shader != nullptr)
    {
        p_active_shader->loadShineVariables(10, 1);
        p_active_shader->loadTransformationMatrix(Matrix4(1.f)); //idnentity matrix
    }
    for(auto&& [id, batch] : a_batches)
    {
        batch->endBatch();
        s_stats.DrawCalls += batch->flushBatch();
        batch->newBatch();

        
    }

    if(p_active_shader != nullptr)
    {
        p_active_shader->dontUse();
        p_active_shader = nullptr;
    }
    //stats
    s_stats.end_time = glfwGetTime();
}

void Renderer::drawLight(std::shared_ptr<Light> light)
{
    assert(p_active_shader != nullptr);
    light->draw(*p_active_shader);
}

void Renderer::startFrameBuffer(Camera &cam, unsigned int width, unsigned int height)
{
    //TODO: RenderbufferObject?
    unsigned int old_height = m_height, old_width = m_width;
    resize(width, height); 
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
    glBindTexture(GL_TEXTURE_2D, m_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!") 
    
    startFrame(cam);
}

unsigned int Renderer::endFrameBuffer()
{
    endFrame();
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
    //resize(old_width, old_height);
    return m_texture;
}

void Renderer::resize(unsigned int width, unsigned int height)
{
    if(width == m_width && height == m_height)
        return;
    m_width = width;
    m_height = height; 
    glViewport(0,0, m_width, m_height); 
    glScissor(0,0, m_width, m_height); 
}

void Renderer::setWindow(GLFWwindow* window)
{
    p_window = window;
    glfwGetWindowSize(p_window, &m_width, &m_height);
    resize(m_height, m_width);
}

void Renderer::addShader(std::string id, Shader* s)
{
    if(p_shaders.empty())    
        p_standard_shader = s;
    p_shaders.emplace(id, s);
    p_shaders.at(id)->use();
    p_shaders.at(id)->getAllUniformLocations();
    p_shaders.at(id)->dontUse();
}

void Renderer::drawCube(Vector3f position, float width, float height, float length, Color color, int mode)
{
    begin(mode);
    
    setColor(color);
    float x = position.x;
    float y = position.y;
    float z = position.z;
    // Front face
    setNormal(Vector3f(0.f, 0.f, 1.f));
    setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Bottom Left
    setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Bottom Right
    setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Top Right
    setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Top Left

    // Back face
    setNormal(Vector3f(0.f, 0.f, -1.f));
    setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Bottom Left
    setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top Left
    setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top Right
    setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Bottom Right

    // Top face
    setNormal(Vector3f(0.f, 1.f, 0.f));
    setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top Left
    setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Bottom Left
    setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Bottom Right
    setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top Right


    // Bottom face
    setNormal(Vector3f(0.f, -1.f, 0.f));
    setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Top Left
    setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Top Right
    setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Bottom Right
    setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Bottom Left

    // Right face
    setNormal(Vector3f(+1.f, 0.f, 0.f));
    setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Bottom Right
    setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top Right
    setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Top Left
    setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Bottom Left

    // Left face
    setNormal(Vector3f(-1.f, 0.f, 0.f));
    setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Bottom Right    
    setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Bottom Left
    setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Top Left
    setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top Right

    end();
}

void Renderer::drawRay(Ray ray, Color color, float length)
{
    drawLine(ray.getOrigin(), ray.getOrigin() + ray.getDirection()*length, color);
    // std::cout << ray.getDirection().x << ", " << ray.getDirection().y << ", " << ray.getDirection().z << std::endl;
}

void Renderer::drawLine(Vector3f a, Vector3f b, Color color)
{
    begin(GL_LINES);
    setColor(color);
    setPoint(a);  // Bottom Left
    setPoint(b);  // Bottom Right
    end();
}

void Renderer::drawQuad(Vector3f pos, Vector3f normal, float width, float height, Color color)
{
    //pos is center of QUAD
    Vector3f plane_vector_a;
    Vector3f plane_vector_b;
    if(normal.y != 0 && normal.z != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(1.f, 0, 0)));
    else if(normal.x != 0 && normal.z != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(0, 1.f, 0)));
    else if(normal.x != 0 && normal.y != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(0, 0, 1.f)));
    else
        std::cout << "Error Null Vector" << std::endl;
    
    
    plane_vector_b = glm::normalize(glm::cross(normal, plane_vector_a));
    plane_vector_a *= width/2;
    plane_vector_b *= height/2;

    begin(GL_QUADS);
    setColor(color);
    setNormal(normal);
    setTextureCoord(Vector2f(0,0));
    setPoint(pos + plane_vector_a + plane_vector_b);
    setTextureCoord(Vector2f(1,0));
    setPoint(pos - plane_vector_a + plane_vector_b);
    setTextureCoord(Vector2f(1,1));
    setPoint(pos - plane_vector_a - plane_vector_b);
    setTextureCoord(Vector2f(0,1));
    setPoint(pos + plane_vector_a - plane_vector_b);
    end();    
}

void Renderer::drawQuad(Vector3f pos, Vector3f normal, float width, float height, unsigned int texture)
{
        //pos is center of QUAD
    Vector3f plane_vector_a;
    Vector3f plane_vector_b;
    if(normal.y != 0 && normal.z != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(1.f, 0, 0)));
    else if(normal.x != 0 && normal.z != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(0, 1.f, 0)));
    else if(normal.x != 0 && normal.y != 0) //normal points
        plane_vector_a = glm::normalize(glm::cross(normal, Vector3f(0, 0, 1.f)));
    else
        std::cout << "Error Null Vector" << std::endl;
    
    plane_vector_b = glm::normalize(glm::cross(normal, plane_vector_a));
    plane_vector_a *= width/2;
    plane_vector_b *= height/2;
    begin(GL_QUADS);
    setColor(Vector4f(1,1,1,1)); //White -> shader will multiply with this
    setTexture(texture);
        // p_active_batch->disableTexture();
    setNormal(normal);
    setTextureCoord(Vector2f(0,0));
    setPoint(pos + plane_vector_a + plane_vector_b);

    setTextureCoord(Vector2f(1,0));
    setPoint(pos - plane_vector_a + plane_vector_b);
    setTextureCoord(Vector2f(1,1));
    setPoint(pos - plane_vector_a - plane_vector_b);
    setTextureCoord(Vector2f(0,1));
    setPoint(pos + plane_vector_a - plane_vector_b);
    end();  
}

void Renderer::drawCubeWire(Vector3f position, float width, float height, float length, Color color)
{
    begin(GL_LINES);
            setColor(color);
            float x = position.x;
            float y = position.y;
            float z = position.z;
            // Front face
            //------------------------------------------------------------------
            // Bottom line
            setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Bottom left
            setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Bottom right

            // Left line
            setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Bottom right
            setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Top right

            // Top line
            setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Top right
            setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Top left

            // Right line
            setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Top left
            setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Bottom left

            // Back face
            //------------------------------------------------------------------
            // Bottom line
            setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Bottom left
            setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Bottom right

            // Left line
            setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Bottom right
            setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top right

            // Top line
            setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top right
            setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top left

            // Right line
            setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top left
            setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Bottom left

            // Top face
            //------------------------------------------------------------------
            // Left line
            setPoint(Vector3f(x - width/2, y + height/2, z + length/2));  // Top left front
            setPoint(Vector3f(x - width/2, y + height/2, z - length/2));  // Top left back

            // Right line
            setPoint(Vector3f(x + width/2, y + height/2, z + length/2));  // Top right front
            setPoint(Vector3f(x + width/2, y + height/2, z - length/2));  // Top right back

            // Bottom face
            //------------------------------------------------------------------
            // Left line
            setPoint(Vector3f(x - width/2, y - height/2, z + length/2));  // Top left front
            setPoint(Vector3f(x - width/2, y - height/2, z - length/2));  // Top left back

            // Right line
            setPoint(Vector3f(x + width/2, y - height/2, z + length/2));  // Top right front
            setPoint(Vector3f(x + width/2, y - height/2, z - length/2));  // Top right back
    end();
}

void Renderer::drawPoint(Vector3f pos, Color color)
{
    begin(GL_POINTS);
    setColor(color);
    setPoint(pos);
    end();

}

}