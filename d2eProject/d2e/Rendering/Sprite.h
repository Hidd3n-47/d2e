#pragma once

namespace d2e
{

struct SpriteInfo
{
    inline explicit SpriteInfo(sf::Texture&& texture)
        : texture(std::move(texture))
        , sprite(sf::Sprite{ this->texture })
    { /* Empty. */ }

    sf::Texture texture;
    sf::Sprite  sprite;
};

} // Namespace d2e.