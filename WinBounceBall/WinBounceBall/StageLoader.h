#pragma once


class StageLoader
{
public:
	// 반환값은 스테이지에서 클리어해야하는 Star 의 개수
	int32 Load(class Scene* scene, std::wifstream& file);
};

