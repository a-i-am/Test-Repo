#pragma once

#include "Singleton.h"

class Texture;

enum class FontSize
{
	Font12 = 12,
	Font18 = 18,
	Font26 = 26,
};

// 리소스 개념이 게임에서 다양하게 등장.
// 텍스처 / 메시 / 사운드 / 이펙트 등등..
// 모든 리소스들을 관리하는 매니저
class ResourceManager : public Singleton<ResourceManager>
{
	friend Singleton<ResourceManager>;
	ResourceManager() {}
public:

	void Init();
	void Destroy();	// 리소스 정리

	Texture* GetTextureInfo(string meshName);
	void LoadTexture(string key, string texturePath, int32 transparent, int32 xFrameCount = 1, int32 yFrameCount = 1);

	HFONT GetFont(FontSize fontSize) 
	{
		auto iter = hFont.find(fontSize);
		if (iter != hFont.end())
			return iter->second;

		return nullptr; 
	}


private:
	unordered_map<string, Texture*> _textures;

	// 사이즈별로 여러개 관리
	unordered_map<FontSize, HFONT> hFont;
};

