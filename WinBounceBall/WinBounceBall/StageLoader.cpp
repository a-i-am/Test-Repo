#include "pch.h"
#include "StageLoader.h"
#include "Scene.h"
#include "Actor.h"
#include "Ball.h"
#include "Block.h"
#include "Star.h"

int32 StageLoader::Load(Scene* scene, std::wifstream& file)
{
    int32 maxStarCount = 0;

    // 이미 추가된 객체들은 싹다 날려버리고, 파일에서 읽은 정보로 씬을 채운다.
    scene->RemoveAllActor();

    std::wstring line;
    while (std::getline(file, line))
    {
        std::wistringstream iss(line);

        int32 actorType;
        iss >> actorType;   // 파일 stream 이 int타입만 읽어서 임시로 actorType int32

        Actor* actor = nullptr;
        switch ((ActorType)actorType)
        {
        case ActorType::AT_BLOCK:
            actor = new Block(Vector(0, 0));
            break;
        case ActorType::AT_BALL:
            actor = new Ball(Vector(0, 0));
            break;
        case ActorType::AT_STAR:
            actor = new Star(Vector(0, 0));
            maxStarCount++;
            break;
        }

        if (actor)
        {
            scene->ReserveAdd(actor);
            actor->LoadActor(iss);
        }
    }

    return maxStarCount;
}
