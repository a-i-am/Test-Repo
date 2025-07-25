#include "pch.h"
#include "ResourceManager.h"
#include "LineMesh.h"
#include "TextureResource.h"

void ResourceManager::Init()
{
    // 모~든 라인메시를 다 로드해서 가지고 있자.
    {
        LineMesh* mesh = new LineMesh();
        mesh->Load(L"Player.txt");
        //mesh->Load("Missile.txt");
        //mesh->Load("Enemy.txt");

        _lineMeshes["Player"] = mesh;
    }

    {
        TextureResource* texture = new TextureResource();
        texture->Load(L"Player.bmp");
        _textures["Player"] = texture;
    }
}

void ResourceManager::Destroy()
{
    for (auto iter : _lineMeshes)
    {
        delete iter.second;
    }
    _lineMeshes.clear();

    for (auto iter : _textures)
    {
        delete iter.second;
    }
    _textures.clear();
}

LineMesh* ResourceManager::GetLineMesh(string meshName)
{
    // 전체적으로 관리하고 있는 모든 라인메시중에 meshName 해당하는 리소스를 반환한다.
    auto iter = _lineMeshes.find(meshName);
    if (iter != _lineMeshes.end())
    {
        return iter->second;
    }
    // 로딩이 안됐거나, 이름이 잘못됐거나
    return nullptr;
}

TextureResource* ResourceManager::GetTexture(string meshName)
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
