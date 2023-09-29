#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "Basic/Vector.hpp"
#include <string>

class Texture
{
    private:
        unsigned int m_id;
        Vector2i m_size;
        unsigned char* m_pixels;
    public:
        Texture();
        Texture(std::string filename);
        ~Texture();
        void loadFromFile(std::string filename);
        const unsigned char* getPixels() const {return m_pixels;}
        void updatePixels(int width, int height, unsigned char* pixels, int channels = 4);
        Vector2i getSize() const {return m_size;}
        int getID() const {return m_id;}
};

#endif /*TEXTURE_HPP*/