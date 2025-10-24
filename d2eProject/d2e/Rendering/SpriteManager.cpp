#include "d2ePch.h"
#include "SpriteManager.h"

namespace d2e
{

std::unique_ptr<SpriteManager> SpriteManager::mInstance = std::make_unique<SpriteManager>();

SpriteManager::~SpriteManager()
{
    for (const Sprite* sprite : mLoadedSprites)
    {
        delete sprite;
    }
}

spriteId SpriteManager::LoadTexture(const std::filesystem::path& path)
{
    if (mLoadedTextures.contains(path))
    {
        DEBUG_WARN("Already loaded in texture at path: {}", path.string().c_str());
        return mLoadedTextures[path];
    }

    mLoadedSprites.emplace_back(new Sprite{ sf::Texture{ path.c_str() } });
    mIdToSpriteIndexMap[mTextureId] = static_cast<uint32_t>(mLoadedSprites.size() - 1);

    return mTextureId++;
}

WeakRef<Sprite> SpriteManager::GetSprite(const spriteId id) const
{
    DEBUG(if (!mIdToSpriteIndexMap.contains(id)) DEBUG_WARN("Failed to obtain sprite with ID {}", id);)

    return WeakRef{ mLoadedSprites[mIdToSpriteIndexMap.at(id)] };
}
} // Namespace d2e.
