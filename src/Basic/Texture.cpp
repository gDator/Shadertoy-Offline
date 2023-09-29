#include "Basic/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glew.h"
#include "logger.hpp"

Texture::Texture()
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, 0);
    LOG_TRACE(m_id);
}

Texture::Texture(std::string filename) : Texture()
{
    loadFromFile(filename);
}

Texture::~Texture()
{
    LOG_TRACE(m_id);
    glDeleteTextures(1, &m_id);
}

void Texture::loadFromFile(std::string filename)
{
    LOG_TRACE(filename); 
	// image.loadFromFile("./res/textures/" + filename);
	// image.flipVertically();
    int channels;
    m_pixels = stbi_load(filename.c_str(), &m_size.x, &m_size.y, &channels, 0);
	updatePixels(m_size.x, m_size.y, m_pixels, channels);
    if(m_size.x == 0 || m_size.y == 0)
        LOG_ERROR("loaded image: " << filename << ": " << m_size.x <<"," << m_size.y);
}

void Texture::updatePixels(int width, int height, unsigned char* pixels, int channels)
{
	m_size.x = width;
	m_size.y = height;
	m_pixels = pixels;
	glBindTexture(GL_TEXTURE_2D, m_id);	

	int format = GL_RGBA;
	if(channels == 4)
		format = GL_RGBA;
	else if(channels = 3)
		format = GL_RGB;
	else
		assert(0!=0);
	glTexImage2D(
		GL_TEXTURE_2D, 0, format, 
		m_size.x, 
		m_size.y, 0,
		format, GL_UNSIGNED_BYTE, 
		m_pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Set some default settings for the texture, so it appears normally.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}