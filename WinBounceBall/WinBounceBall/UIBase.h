#pragma once

class UIBase
{
public:
	UIBase(Vector pos);
	virtual ~UIBase();

	virtual void Init() {}
	virtual void Update() {}
	virtual void Render(HDC hdc);
	virtual void Open() { _isOpen = true; }
	virtual void Close() { _isOpen = false; }
	
	bool IsOpen() const { return _isOpen; }
	void SetOpen(bool open) { _isOpen = open; }

protected:
	bool _isOpen = true;
	Vector _pos = {};
};

