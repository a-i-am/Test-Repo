#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Game.h"

void ResourceManager::Init()
{
    // 외부 폰트를 시스템에 임시로 등록
    //FR_PRIVATE: 시스템 전체가 아닌 현재 프로세스에서만 사용
    //AddFontResourceEx((directory / L"Font/MaplestoryLight.ttf").c_str(), FR_PRIVATE, 0);

    // win API 
    auto createMyFont = [this](FontSize fontSize)
        {
            hFont[fontSize] = CreateFont(
                (int32)fontSize, 0, 0, 0, FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                L"메이플스토리"  // Family 이름!
            );
        };

    createMyFont(FontSize::Font12);
    createMyFont(FontSize::Font18);
    createMyFont(FontSize::Font26);
     
    // 씬마다 필요한 텍스처를 정해서 로딩한다.
    //{
    //    Texture* texture = new Texture();
    //    texture->Load(L"Player.bmp", RGB(252, 0, 255));
    //    _textures["Player"] = texture;
    //}
    //{
    //    Texture* texture = new Texture();
    //    texture->Load(L"PlayerBullet.bmp", RGB(255, 0, 255));
    //    _textures["PlayerBullet"] = texture;
    //}
}

void ResourceManager::Destroy()
{
    for (auto iter : _textures)
    {
        delete iter.second;
    }
    _textures.clear();
}

Texture* ResourceManager::GetTextureInfo(string meshName)
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

void ResourceManager::LoadTexture(string key, string texturePath, int32 transparent, int32 xFrameCount, int32 yFrameCount)
{
    Texture* texture = new Texture();
    texture->Load(key, texturePath, transparent, xFrameCount, yFrameCount);
    _textures[key] = texture;
}