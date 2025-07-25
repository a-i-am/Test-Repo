#pragma once

#include "DataObject.h"

class ResourceData : public DataObject
{
public:
	class Item
	{
	public:
		string key;
		string fileName;
		int32 transparent = -1;
		int32 countX = 1;
		int32 countY = 1;
	};

public:
	virtual ~ResourceData();

	static string Key()
	{
		return "ResourceData";
	}
	virtual void Load(json data) override;
	virtual string GetFileName() override
	{
		return "ResourceData.json";
	}

	unordered_map<string, Item*> _gameSceneData;
};

