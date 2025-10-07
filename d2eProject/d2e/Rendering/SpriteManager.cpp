#include "d2ePch.h"
#include "SpriteManager.h"

namespace d2e
{

std::unique_ptr<SpriteManager> SpriteManager::mInstance = std::make_unique<SpriteManager>();

spriteId SpriteManager::LoadTexture(const std::filesystem::path& path)
{
    mLoadedTextures.emplace_back(path.c_str());
    mLoadedSprites.emplace_back(mLoadedTextures.back());
    mIdToSpriteIndexMap[mTextureId] = static_cast<uint32_t>(mLoadedSprites.size() - 1);

    return mTextureId++;
}

} // Namespace d2e.
