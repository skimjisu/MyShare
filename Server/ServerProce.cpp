#pragma hdrstop
#include "ServerProce.h"
#pragma package(smart_init)

void __fastcall SendWebBodyPage(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container) {

	AnsiString NowPath, FileName, bodyHTML, ShereListHTML, ShereListItemHTML;
	int i = 0;
	NowPath = ExtractFilePath(ParamStr(0));

	while (true) {
		if (WEB_FILE_LIST[i] != NULL)
			if (!FileExists(NowPath + WEB_FILE_LIST[i]))
				return;
			else
				break;
		i++;
	}

	FileName = NowPath + WEB_FILE_LIST[0];
	TStringStream *StrStrem = new TStringStream("", TEncoding::UTF8, true);
	StrStrem->LoadFromFile(FileName);
	bodyHTML = StrStrem->DataString;
	StrStrem->Clear();

	FileName = NowPath + WEB_FILE_LIST[1];
	StrStrem->LoadFromFile(FileName);
	ShereListHTML = StrStrem->DataString;
	StrStrem->Clear();

	FileName = NowPath + WEB_FILE_LIST[2];
	StrStrem->LoadFromFile(FileName);
	ShereListItemHTML = StrStrem->DataString;
	StrStrem->Clear();

	delete StrStrem;

	AResponseInfo->ContentType = "text/html";
	AResponseInfo->CharSet = "utf-8"; // "euc-kr"  ,"utf-8"  ;
	AResponseInfo->ContentText = AnsiToUtf8(Container->MakeShereListToHTML(bodyHTML, ShereListHTML, ShereListItemHTML));
}

void __fastcall SendFileToWeb(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params) {
	int number = StrToInt(GetParamNameToValue(Params, "number"));
	AnsiString MD5 = GetParamNameToValue(Params, "id");
	TShereItem *Item = Container->FindShereItem(number, MD5);
	if (Item == NULL)
		return;
	AResponseInfo->CharSet = "utf-8"; // "euc-kr"  ,"utf-8"  ;
	AResponseInfo->ContentType = "application/octet-stream;charset=UTF-8";
	TFileStream *FileStream = new TFileStream(Item->FileName, fmOpenRead | fmShareDenyWrite);
	FileStream->Position = 0;
	AResponseInfo->AcceptRanges = "bytes";
	AResponseInfo->ContentRangeEnd = FileStream->Size;
	AResponseInfo->ContentLength = FileStream->Size;
	AResponseInfo->ContentDisposition = "attachment; filename=\"" + TIdURI::PathEncode(ExtractFileName(Item->FileName),
		IndyTextEncoding(TEncoding::UTF8)) + "\";";
	AResponseInfo->FreeContentStream = true;
	AResponseInfo->ContentStream = FileStream;
}

void __fastcall SendFileSizeFromHTTPHeadInfo(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params) {
	int number = StrToInt(GetParamNameToValue(Params, "number"));
	AnsiString MD5 = GetParamNameToValue(Params, "id");
	TShereItem *Item = Container->FindShereItem(number, MD5);
	if (Item != NULL)
		AResponseInfo->ContentLength = Item->FileSize;
	else
		AResponseInfo->ContentLength = 0;
}

void __fastcall SendShereList(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container) {
	AResponseInfo->ContentType = "text/html";
	AResponseInfo->CharSet = "euc-kr"; // "euc-kr"  ,"utf-8"  ;
	AResponseInfo->ResponseNo = 200;
	AResponseInfo->ContentText = Container->GetShereListToString();
}

void __fastcall SendShereItemList(TIdHTTPResponseInfo *AResponseInfo, TContainer *Container, TStrings *Params) {
	int Num = -1;
	try {
		Num = StrToInt(Params->Strings[0]);
	}
	catch (...) {
		Num = -1;
	}
	if (Num == -1)
		return;
	AResponseInfo->ContentType = "text/html";
	AResponseInfo->CharSet = "euc-kr"; // "euc-kr"  ,"utf-8"  ;
	AResponseInfo->ResponseNo = 200;
	AResponseInfo->ContentText = Container->GetShereListItemToString(Num);
}

void __fastcall SendRequestFileWeb(AnsiString Document, TIdHTTPResponseInfo *AResponseInfo) {
	AnsiString FileName = ExtractFileDir(Application->ExeName) + Document;
	FileName = AnsiReplaceStr(FileName, "/", "\\");
	if (FileExists(FileName)) {
		TFileStream *FileStream = new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);
		FileStream->Position = 0;
		AResponseInfo->AcceptRanges = "bytes";
		AResponseInfo->ContentRangeEnd = FileStream->Size;
		AResponseInfo->ContentLength = FileStream->Size;
		AResponseInfo->FreeContentStream = true;
		AResponseInfo->ContentStream = FileStream;
	}
	else {
		AResponseInfo->ContentText = "Not File System !!!";
	}
}
/*
 void __fastcall SendFileToClient(AnsiString Ranges,TIdHTTPResponseInfo *AResponseInfo,TContainer *Container,TStrings *Params){

 int number = StrToInt(GetParamNameToValue(Params,"number"));
 AnsiString MD5 = GetParamNameToValue(Params,"id");
 TShereItem *Item = Container->FindShereItem(number,MD5);
 if(Item == NULL )return;
 AResponseInfo->ContentType = "application/octet-stream;charset=UTF-8";


 __int64 StartPos = -1;
 __int64 EndPos = -1;
 __int64 ContentsSize;
 AnsiString s,e,n;

 int dash_pos = Ranges.Pos("-");
 s = Ranges.SubString(0, dash_pos);
 e = Ranges.SubString(dash_pos + 1,Ranges.Length());

 try {
 StartPos = StrToInt(Ranges.SubString(0, dash_pos));
 EndPos = StrToInt(Ranges.SubString(dash_pos + 1,Ranges.Length()));
 } catch (...) {		}


 if(!TryStrToInt64(s,StartPos) || !TryStrToInt64(e,EndPos))return;
 if (  (EndPos - StartPos) <= 0    ){
 ContentsSize =  Item->FileSize - StartPos;
 }
 else ContentsSize = EndPos - StartPos;

 //		TMemoryStream *FileStream = new TMemoryStream;
 TFileStream *FileStream = new TFileStream(Item->FilePath, fmOpenRead | fmShareDenyWrite);
 //		FileStream->LoadFromFile(Item->FilePath);
 FileStream->Position = StartPos;

 TMemoryStream *SendStream = new TMemoryStream;

 try { 	SendStream->CopyFrom(FileStream,ContentsSize);
 } catch (...) { 			}

 delete FileStream;
 SendStream->Position = 0;

 AResponseInfo->AcceptRanges = "bytes";
 AResponseInfo->ContentRangeEnd = SendStream->Size;
 AResponseInfo->ContentLength = SendStream->Size;
 AResponseInfo->ContentDisposition = "Content-Disposition: attachment; filename=\"" + Item->FileName +  "\"";
 AResponseInfo->FreeContentStream = true;
 AResponseInfo->ContentStream = SendStream;













 TFileStream *FileStream = new TFileStream(Item->FileName, fmOpenRead | fmShareDenyWrite);
 __int64 StartPos = 0, EndPos = 0;

 if(Ranges.Length() >= 1)
 {
 int dash_pos = Ranges.Pos("-");
 try {
 StartPos = StrToInt(Ranges.SubString(0, dash_pos));
 EndPos = StrToInt(Ranges.SubString(dash_pos + 1,Ranges.Length()));
 } catch (...) {		}

 if(EndPos == 0)EndPos = FileStream->Size;  // Endless Range: StartPos-
 }
 else
 {
 StartPos = 0;
 EndPos = FileStream->Size;
 }

 if(Ranges.Length() >= 1)
 {
 AResponseInfo->ContentRangeStart = StartPos;
 AResponseInfo->ContentRangeEnd = EndPos;
 AResponseInfo->ResponseNo = 206;
 AResponseInfo->ContentRangeInstanceLength = EndPos + 1;
 AResponseInfo->ContentLength = EndPos - StartPos + 1;
 AResponseInfo->AcceptRanges = "bytes";
 }

 FileStream->Position = StartPos;
 AResponseInfo->ContentLength = FileStream->Size;
 AResponseInfo->ContentStream = FileStream;
 AResponseInfo->FreeContentStream = true;


 }

 */
