#include "pch.h"
#include "DataManager.h"
#include "ResourceManager.h"
#include "ResourceData.h"

void DataManager::Init()
{
	// 각종 모든 데이터 로드
	fs::path directory = fs::current_path() / L"../Resources/Data";

    // Resource 
    {
        ResourceData* data = new ResourceData();
        loadDataObject(directory, ResourceData::Key(), data);
    }

    // Item..등등 여러가지 로드
    {

    }
}

void DataManager::Destory()
{
    for (auto iter : _datas)
    {
        SAFE_DELETE(iter.second);
    }
    _datas.clear();
}

void DataManager::loadDataObject(fs::path directory, string key, DataObject* obj)
{
    if (_datas.find(key) != _datas.end())
    {
        // 이미 존재하는 키라면 리턴
        return;
    }

    fs::path path = directory / obj->GetFileName();
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        MessageBox(nullptr, L"Failed to open JSON file", L"Error", MB_OK);
        return;
    }

    json data = json::parse(file);
    obj->Load(data);

    _datas.emplace(key, obj);
}