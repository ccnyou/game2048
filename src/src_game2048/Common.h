#pragma once

#include <memory>

class Common
{
public:
	static std::auto_ptr<Gdiplus::Image>	ImageFromResource(LPCTSTR name, LPCTSTR type, HMODULE hInst = NULL);
};
