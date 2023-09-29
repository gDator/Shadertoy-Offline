
#include <map>
#include <string>
#include "Basic/TextureAtlas.hpp"
#include "logger.hpp"


TextureAtlas::TextureAtlas()
{

}

/*
    @name: loadTexture
    @func: Add a Texture
    @args: @name Shortcutname to access texture, @filename Path to Texture
    @return: none
*/
void TextureAtlas::loadTexture(const std::string& name, const std::string& filename)
{

    // tex.loadFromFile(filename);
    textures.insert_or_assign(name, std::make_shared<Texture>(filename));
    LOG_TRACE(name << ": " << textures[name]->getID());
}
/*
    @name: addTexture
    @func: Add a Texture manually
    @args: @name Shortcutname to access texture, @texture is the Texture itself
    @return: none
*/
void TextureAtlas::addTexture(const std::string& name, Texture& tex)
{
    textures.insert_or_assign(name, std::make_unique<Texture>(tex));
}


/*
    @name: getRef
    @func: Access a texture via name
    @args: @name Shortcutname to access texture, @filename Path to Texture
    @return: none
*/
const std::shared_ptr<Texture>& TextureAtlas::getRef(const std::string& name) const
{
    return textures.at(name);
}

int TextureAtlas::getAmountOfTextures()
{
    return textures.size();
}