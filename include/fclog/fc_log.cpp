#include "StdAfx.h"
#include "fc_log.h"


void dbg_log::FC_logA( const char * pszFormat, ... )
{
	LPSTR pEnd = NULL;
	size_t nRemain = 0;
	char szText[1024] = {0};

	SYSTEMTIME localTime = {0};
	GetLocalTime(&localTime);

	StringCchPrintfExA(szText, sizeof(szText), &pEnd, &nRemain, 0,
		"[%04d-%02d-%02d][%02d:%02d:%02d:%03d][%d][%d]", localTime.wYear,
		localTime.wMonth, localTime.wDay, localTime.wHour,
		localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
		GetCurrentProcessId(), GetCurrentThreadId());

	va_list vlArgs;
	va_start(vlArgs, pszFormat);
	StringCchVPrintfExA(pEnd, nRemain, &pEnd, &nRemain, 0, pszFormat, vlArgs);
	va_end(vlArgs);
	if ( nRemain >= 2 )
	{
		StringCchCopyA(pEnd, nRemain, "\n");
	}
	OutputDebugStringA(szText);
#ifdef FC_LOG_TO_File
	char szFile[MAX_PATH]={0};
	GetModuleFileNameA(NULL, szFile, MAX_PATH);
	StringCchCatA(szFile, MAX_PATH, ".log");
	OutputToFile(szFile, szText);
#endif
}

short dbg_log::OutputToFile( LPCSTR strFile, LPCSTR szText )
{
	FILE *file;
	if (fopen_s( &file, strFile, "a+") == 0)
	{
		fseek(file,0,SEEK_END);
		fwrite(szText, strlen(szText), sizeof(char), file);//把字符串内容写入到文件
		fclose(file);
	}


	return 1;
}


