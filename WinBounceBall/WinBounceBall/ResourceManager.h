#pragma once

#include "Singleton.h"

class Texture;

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

	void LoadTexture(string name, wstring path, int32 countX = 1, int32 countY = 1);
	Texture* GetTexture(string name);

	const SpriteInfo* GetSpriteInfo(string key);

private:
	unordered_map<string, Texture*> _textures;

	// sprite info
	// Ball_0 -> 노란색 기본공
	// Blokck_0 -> 회색 기본 블럭
	// 
	// json 파일에서 추출한다면
	// Ball_Basic -> 노란색 기본공
	// Block_Basic -> 회색 기본 블럭
	unordered_map<string, SpriteInfo>	_spriteNames;
};

