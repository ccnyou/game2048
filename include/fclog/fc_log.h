#ifndef _FC_LOG_ 
#define _FC_LOG_ 

#pragma warning(disable:4995)
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include "DebugConfig.h"

#define FC_LOG_TO_File
using namespace std;
class dbg_log
{
private:
	static string m_outPutFile;

public:
	static void FC_logA(const char * pszFormat, ...);
	static short OutputToFile(LPCSTR strFile, LPCSTR szText);

};

#if USING_FC_LOG

#define _FC_LOG	dbg_log::FC_logA

#else

#define _FC_LOG	/\
/


#endif

#endif