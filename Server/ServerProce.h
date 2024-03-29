#ifndef ServerProceH
#define ServerProceH

#include <System.Classes.hpp>
#include <IdCustomHTTPServer.hpp>
#include <IdURI.hpp>

#include "..\Common.h"
#include "Container.h"

static const char *WEB_FILE_LIST[] = {"index.html", "shere_list_tmpl.html", "shere_list_item_tmpl.html", NULL};

void __fastcall SendWebBodyPage(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container);
void __fastcall SendFileToWeb(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params);
void __fastcall SendFileSizeFromHTTPHeadInfo(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params);
void __fastcall SendShereList(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container);
void __fastcall SendShereItemList(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params);
void __fastcall SendRequestFileWeb(AnsiString Document, TIdHTTPResponseInfo *AResponseInfo);
// void __fastcall SendFileToClient(AnsiString Ranges,TIdHTTPResponseInfo *AResponseInfo,TContainer *Container,TStrings *Params);

#endif
