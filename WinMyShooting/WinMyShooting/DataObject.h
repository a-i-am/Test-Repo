#pragma once

class DataObject
{
public:
	virtual ~DataObject() {}

	virtual void Load(json data) {}
	virtual string GetFileName() { return ""; }
};

std::string Utf8ToAnsi(const std::string& utf8Str);
std::wstring Utf8ToWide(const std::string& utf8Str);