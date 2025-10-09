#pragma once

#include <filesystem>

#include "d2e/Core/Engine.h"
#include "d2e/Rendering/Sprite.h"

namespace d2e
{

typedef uint32_t spriteId;

class SpriteManager
{
public:
    ~SpriteManager();

    [[nodiscard]] inline static WeakRef<SpriteManager> Instance() { return WeakRef{ mInstance.get() }; }

    [[nodiscard]] spriteId LoadTexture(const std::filesystem::path& path);

    //todo make it return invalid texture if it doesn't exist.
    [[nodiscard]] inline WeakRef<Sprite> GetSprite(const spriteId id) const
        { DEBUG(if (!mIdToSpriteIndexMap.contains(id)) DEBUG_WARN("Failed to obtain sprite with ID {}", id);) return WeakRef{ mLoadedSprites[mIdToSpriteIndexMap.at(id)] }; }
private:
    static std::unique_ptr<SpriteManager> mInstance;

    std::vector<Sprite*> mLoadedSprites;
    std::unordered_map<std::filesystem::path, spriteId> mLoadedTextures;
    std::unordered_map<spriteId, uint32_t> mIdToSpriteIndexMap;

    spriteId mTextureId{ 0 };
};

} // Namespace d2e.
