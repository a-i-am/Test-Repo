#include "pch.h"
#include "ResourceData.h"

ResourceData::~ResourceData()
{
	for (auto iter : _gameSceneData)
	{
		SAFE_DELETE(iter.second);
	}
	_gameSceneData.clear();
}

void ResourceData::Load(json data)
{
	// 우리만의 자료형으로 변경
	auto gameSceneData = data["GameScene"];
	for (json::iterator it = gameSceneData.begin(); it != gameSceneData.end(); ++it)
	{
		auto key = it.key();	// "BG"
		auto value = it.value(); // "{ }"

		ResourceData::Item* item = new ResourceData::Item();
		item->key = key;
		item->fileName = value["fileName"];

		// json 은 utf8 포맷으로 읽으니깐, 원하는 타입으로 변환해줘야한다.
		//wstring desc = Utf8ToWide(value["desc"]);

		if (value.contains("countX"))
		{
			item->countX = value["countX"];
		}

		if (value.contains("countY"))
		{
			item->countY = value["countY"];
		}

		if (value.contains("transparent"))
		{
			item->transparent = RGB(value["transparent"][0], value["transparent"][1], value["transparent"][2]);
		}

		_gameSceneData.insert(make_pair(item->key, item));
	}
}
