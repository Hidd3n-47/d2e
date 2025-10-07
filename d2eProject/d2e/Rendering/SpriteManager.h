#pragma once

#include <filesystem>

#include "d2e/Core/Engine.h"

namespace d2e
{

typedef uint32_t spriteId;

class SpriteManager
{
public:
    [[nodiscard]] static inline WeakRef<SpriteManager> Instance() { return WeakRef{ mInstance.get() }; }

    [[nodiscard]] spriteId LoadTexture(const std::filesystem::path& path);

    //todo make it return invalid texture if it doesn't exist.
    [[nodiscard]] inline sf::Sprite GetSprite(const spriteId id) const { DEBUG(if (!mIdToSpriteIndexMap.contains(id)) DEBUG_WARN("Failed to obtain sprite with ID {}", id);) return mLoadedSprites[mIdToSpriteIndexMap.at(id)]; }
private:
    static std::unique_ptr<SpriteManager> mInstance;

    std::vector<sf::Sprite>  mLoadedSprites;
    std::vector<sf::Texture> mLoadedTextures;
    std::unordered_map<spriteId, uint32_t> mIdToSpriteIndexMap;

    spriteId mTextureId{ 0 };
};

} // Namespace d2e.
