#pragma hdrstop
#include "Common.h"
#pragma package(smart_init)

AnsiString __fastcall GetParamNameToValue(TStrings *List,AnsiString ParamName){
	int p1 = List->IndexOfName(ParamName);
	if( p1 == -1 )return "";
	AnsiString Param = List->Strings[p1];
	p1 = Param.Pos("=");
	if (p1 == 0 )return "";
	return Param.SubString( p1 +1, Param.Length() - p1 ).Trim();
}

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



void __fastcall SetFormStyleNone(TForm *form,bool set){
	if(set){
		form->BorderStyle = bsNone;

		int Style = GetWindowLong(form->Handle, GWL_STYLE);
		SetWindowLong(form->Handle, GWL_STYLE, Style & (~WS_CAPTION ) | (WS_BORDER) &(~WS_DLGFRAME));
		form->Height -= GetSystemMetrics(SM_CYCAPTION);
		form->Refresh();
	}
	else {
		form->BorderStyle = bsSingle;
    }
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData){
	if(uMsg==BFFM_INITIALIZED)
		if(lstrlenA((char*)lpData))SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
	return 0;
}

AnsiString __fastcall BrowseFolder(HWND hwnd, AnsiString Title, AnsiString Folder){
	BROWSEINFOA Info;
	LPITEMIDLIST IDList;
	char Result[1024]="";
	char s[256];
	memset(&Info, 0, sizeof(Info));
	Info.hwndOwner = hwnd;

	Info.pidlRoot = NULL;
	lstrcpyA(s, Title.c_str());
	Info.lpszTitle = s;
	Info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_NEWDIALOGSTYLE;
	if(Folder.Length())lstrcpyA(Result, Folder.c_str());
	Info.lParam=(UINT)Result;
	Info.lpfn=BrowseCallbackProc;
	IDList = SHBrowseForFolderA(&Info);
	if(IDList){
		memset(Result, 0, sizeof(Result));
		SHGetPathFromIDListA(IDList, Result);
		CoTaskMemFree(IDList);
	}else lstrcpyA(Result,"");


	AnsiString stTemp = AnsiString(Result).Trim();

	if(stTemp == "")return "";
	else if(stTemp.Length() > 3)return stTemp + "\\";
	else return stTemp;
//	if (AnsiString(Result).Trim() != "")return AnsiString(Result)+"\\";
//	else return "";
}
