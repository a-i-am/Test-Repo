#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"

void ResourceManager::Init()
{
    auto createSpriteNameInfo = [this](string spriteName, int32 xCount, int32 totalCount, string bitmapKey)
        {
            for (int32 i = 0; i < totalCount; ++i)
            {
                int32 x = i % xCount;
                int32 y = i / xCount;
                _spriteNames.emplace(std::format("{0}_{1}", spriteName, i), SpriteInfo{ x, y, 0, false, bitmapKey });
            }
        };

    createSpriteNameInfo("Ball", 2, 8, "Ball"); // 원래는 2,4 개수가 맞지만, 사용할 공만 개수를 정했다.
    createSpriteNameInfo("Block", 3, 17, "Block");
    createSpriteNameInfo("Star", 1, 3, "Star");

    _spriteNames.emplace("EatStarEffect", SpriteInfo{ 0, 0, 2.0f, false, "EatStarEffect" });
    _spriteNames.emplace("DeadBall", SpriteInfo{ 0, 0,2.0f, false, "DeadBall" });
}

void ResourceManager::Destroy()
{
    for (auto iter : _textures)
    {
        delete iter.second;
    }
    _textures.clear();
}

void ResourceManager::LoadTexture(string name, wstring path, int32 countX, int32 countY)
{
    Texture* texture = new Texture();
    texture->Load(path, countX, countY, RGB(255, 255, 255));
    _textures[name] = texture;
}

Texture* ResourceManager::GetTexture(string meshName)
{
    // 전체적으로 관리하고 있는 모든 라인메시중에 meshName 해당하는 리소스를 반환한다.
    auto iter = _textures.find(meshName);
    if (iter != _textures.end())
    {
        return iter->second;
    }
    // 로딩이 안됐거나, 이름이 잘못됐거나
    return nullptr;
}

const SpriteInfo* ResourceManager::GetSpriteInfo(string key)
{
    if (_spriteNames.find(key) != _spriteNames.end())
    {
        // 이미 존재하는 키라면 리턴
        return &_spriteNames[key];
    }
    return nullptr;
}
