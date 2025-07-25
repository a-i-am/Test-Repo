#include "pch.h"
#include "LineMesh.h"

void LineMesh::Load(wstring fileName)
{
	fs::path path = fs::current_path();
	path += L"\\" + fileName;

	wifstream file;
	file.open(path);

	// 라인 개수
	int32 count;
	file >> count;

	_lines.clear();

	for (int32 i = 0; i < count; i++)
	{
		POINT pt1;
		POINT pt2;

		wstring str;
		file >> str;
		::swscanf_s(str.c_str(), L"(%d,%d)->(%d,%d)", &pt1.x, &pt1.y, &pt2.x, &pt2.y);

		_lines.push_back(make_pair(pt1, pt2));
	}

	file.close();
}

void LineMesh::Render(HDC hdc, Vector pos)
{
	for (auto& line : _lines)
	{
		POINT pt1 = line.first;
		POINT pt2 = line.second;

		Vector pos1;
		pos1.x = pos.x + (float)pt1.x;
		pos1.y = pos.y + (float)pt1.y;

		Vector pos2;
		pos2.x = pos.x + (float)pt2.x;
		pos2.y = pos.y + (float)pt2.y;

		::MoveToEx(hdc, static_cast<int32>(pos1.x), static_cast<int32>(pos1.y), nullptr);
		::LineTo(hdc, static_cast<int32>(pos2.x), static_cast<int32>(pos2.y));
	}
}
