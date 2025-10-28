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

    [[nodiscard]] WeakRef<SpriteInfo> GetSprite(const spriteId id) const;

private:
    static std::unique_ptr<SpriteManager> mInstance;

    std::vector<SpriteInfo*> mLoadedSprites;
    std::unordered_map<spriteId, uint32_t> mIdToSpriteIndexMap;
    std::unordered_map<std::filesystem::path, spriteId> mLoadedTextures;

    spriteId mTextureId{ 0 };
};

} // Namespace d2e.
