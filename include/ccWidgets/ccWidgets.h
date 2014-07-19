#ifndef ccWidgets_h__
#define ccWidgets_h__

#ifdef _WIN32
//拒绝 Windows.h 自己包含 Winsock.h
#define _WINSOCKAPI_

//其他项目引用
#ifndef _CC_WIDGETS_
#   ifdef _DEBUG
#       pragma warning(disable:4099)
#pragma message("引入 ccWidgets 2005 库")
#			ifdef	_UNICODE
#				pragma comment(lib,"..\\..\\lib\\ccWidgets05\\ccWidgets05_ud.lib")
#			else   // _UNICODE
#				pragma comment(lib,"..\\..\\lib\\ccWidgets05\\ccWidgets05_d.lib")
#			endif	// _UNICODE
#   else // Release
#       pragma warning(disable:4099)
#pragma message("引入 ccWidgets 2005 库")
#			ifdef	_UNICODE
#				pragma comment(lib,"..\\..\\lib\\ccWidgets05\\ccWidgets05_u.lib")
#			else   // _UNICODE
#				pragma comment(lib,"..\\..\\lib\\ccWidgets05\\ccWidgets05.lib")
#			endif	// _UNICODE
#	endif // _DEBUG

#endif // _CC_WIDGETS_
#endif

#include "Random.h"

#endif // ccWidgets_h__
