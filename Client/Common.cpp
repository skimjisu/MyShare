#pragma hdrstop
#include "Common.h"
#pragma package(smart_init)

AnsiString __fastcall MakeFileSizeString(__int64 Size){
	if (Size > 1073741824 )return FormatFloat("##.## GB",(double)Size / 1024 /1024 /1024);
	if (Size > 1048576)return FormatFloat("##.## MB",(double)Size / 1024 /1024);
	if (Size > 1024)return FormatFloat("##.## KB",(double)Size / 1024);
	if (Size < 1024)return IntToStr( Size ) + " Bytes";
	return "";
}


__int64 __fastcall GetLocalFileSize(AnsiString FileName){
	DWORD dwLow, dwHigh;
	__int64 FileSize = 0;
	HANDLE hFile = ::CreateFileA( FileName.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile != INVALID_HANDLE_VALUE ){
		dwLow = ::GetFileSize( hFile, &dwHigh );
		if( (dwLow != 0xFFFFFFFF) || GetLastError() == NO_ERROR )FileSize = (__int64)dwHigh << 32 | dwLow;
		::CloseHandle( hFile );
	}
	return FileSize;
}





