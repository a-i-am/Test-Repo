#pragma once

#include "Singleton.h"

class DataObject;

class DataManager : public Singleton<DataManager>
{
	friend Singleton<DataManager>;

private:
	DataManager() {}

public:
	void Init();
	void Destory();

	template<typename T>
	const T* GetData(string key) const
	{
		auto iter = _datas.find(key);
		if (iter != _datas.end())
		{
			return dynamic_cast<T*>(iter->second);
		}
		return nullptr;
	}

private:
	void loadDataObject(fs::path directory, string key, DataObject* obj);

private:

	// N 개의 데이터를 관리
	// "Resource" -> ResourceData
	// "Item" -> ItemData
	map<string, DataObject*>  _datas;
};

