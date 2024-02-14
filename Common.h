#ifndef CommonH
#define CommonH

#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>
//#include <shlobj.h>
#include <Winapi.Shlobj.HPP>


#define	TOKEN								"|"
#define	TOKEN2								">"
#define	TOKEN3								"<>"

#define DOWNLOADER_USER_AGENT_STRING		"MyShere HTTP Protocol"


AnsiString __fastcall MakeFileSizeString(__int64 Size);
__int64 __fastcall GetLocalFileSize(AnsiString FileName);
void __fastcall SetFormStyleNone(TForm *form,bool set);
AnsiString __fastcall GetParamNameToValue(TStrings *List,AnsiString ParamName);

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
AnsiString __fastcall BrowseFolder(HWND hwnd, AnsiString Title, AnsiString Folder);

#endif
