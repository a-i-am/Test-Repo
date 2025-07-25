#pragma once

// 라인으로 그려지는 메시
// 모든 리소스는 공유될수 있기 떄문에, 별도의 클래스로 관리.
// 내 비행기 쓸수있고.. 적 비행기도 쓸수


class LineMesh
{
public:
	void Load(wstring fileName);

	// 해당 리소스가 그려지는 부분
	void Render(HDC hdc, Vector pos);

private:
	vector<pair<POINT, POINT>> _lines;	// 일종의 리소스를 만들수 있다.
};

