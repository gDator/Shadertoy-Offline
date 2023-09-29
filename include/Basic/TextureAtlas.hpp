#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "Basic/Texture.hpp"
#include <map>
#include <memory>

class TextureAtlas
{
    public:
        TextureAtlas();
    private:
        std::map<std::string, std::shared_ptr<Texture>> textures;
    public:

        void loadTexture(const std::string &name, const std::string &filename);
        void addTexture(const std::string &name, Texture& tex);
        const std::shared_ptr<Texture>& getRef(const std::string &texture) const;
        int getAmountOfTextures();
};
#endif /*TEXTURE_ATLAS*/