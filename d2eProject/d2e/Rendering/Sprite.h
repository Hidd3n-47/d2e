#pragma once

namespace d2e
{

struct Sprite
{
    inline explicit Sprite(sf::Texture&& texture)
        : texture(std::move(texture))
        , sprite(sf::Sprite{ this->texture })
    { /* Empty. */ }

    sf::Texture texture;
    sf::Sprite  sprite;
};

} // Namespace d2e.