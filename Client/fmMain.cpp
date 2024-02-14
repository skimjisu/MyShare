#include <vcl.h>
#pragma hdrstop

#include "fmMain.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

__int64 EndPos = 0,TotalFileSize = 0,ReadPerSec = 0;;
int NowWorkPos = 0;
int LastSecendCount;

__fastcall TMainForm::TMainForm(TComponent* Owner)	: TForm(Owner){

}

void __fastcall TMainForm::FormCreate(TObject *Sender){
	try {	Shere_ini = new TIniFile(ExtractFilePath( ParamStr(0) )+ "ShereInfo.ini");	}
	catch (...) {			return;		}

	TListColumn *Item = LV_ItemList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "                                   파                   일                   명";
	Item->Width = 350;


	Item = LV_ItemList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "크 기";
	Item->Width = 70;

	Item = LV_ItemList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "완 료";
	Item->Width = 50;

	SysIconList = new TExtIconList;
	LV_ItemList->SmallImages = SysIconList->IconList;

}


void __fastcall TMainForm::FormDestroy(TObject *Sender){
	delete Shere_ini;
	delete SysIconList;
}

void __fastcall TMainForm::Btn_ChoiceFileClick(TObject *Sender){
	if(!Dlg_ChoiceFile->Execute())return;
	for (int i = 0; i < Dlg_ChoiceFile->Files->Count; i++) {
		AddListItem(Dlg_ChoiceFile->Files->Strings[i] );
	}
}

void __fastcall TMainForm::Btn_StartClick(TObject *Sender){

	TotalFileSize = 0;

	for (int i = 0; i < LV_ItemList->Items->Count; i++) {
		TMyListItem *Item =(TMyListItem *)LV_ItemList->Items->Item[i]->Data;
		TotalFileSize += Item->FileSize;
	}

	NowWorkPos = 0;
	EndPos = 0;

	LastSecendCount = 1;


	LB_TotalProce->Caption = "0 / " + IntToStr(LV_ItemList->Items->Count);

	FileWorkerTimer->Enabled = true;

}
void __fastcall TMainForm::FileWorkerTimerTimer(TObject *Sender){

	FileWorkerTimer->Enabled = false;
	if(	LV_ItemList->Items->Count <= NowWorkPos){
		return;
	}

	TMyListItem *Item =(TMyListItem *)LV_ItemList->Items->Item[NowWorkPos]->Data;
	LB_NowFile->Caption = Item->FileName;

	if(Item->Stat == 10){
		EndPos += Item->FileSize;
		TotalWorkProce();
	}
	else {
		GetMD5HashProce(NowWorkPos);

	}

	LB_TotalProce->Caption = IntToStr(NowWorkPos + 1) + " / " + IntToStr(LV_ItemList->Items->Count);


	NowWorkPos ++;
	FileWorkerTimer->Enabled = true;
}


void __fastcall TMainForm::ReadPerSecTimerTimer(TObject *Sender){
	ReadPerSecProce();
	LastSecendCount++;
}

void __fastcall TMainForm::TotalWorkerTimerTimer(TObject *Sender){
	TotalWorkProce();

}

void __fastcall TMainForm::TotalWorkProce(){
	PB_WorkerProce->Position = (int)(((double)EndPos / (double)TotalFileSize) * 100);
//		double Per = (double)EndPos / (double)TotalFileSize * 100;
	LB_TotalPercent->Caption = FormatFloat("00.00 %", (double)EndPos / (double)TotalFileSize * 100 );
	LB_TotalSize->Caption = MakeFileSizeString( EndPos ) + " / " + MakeFileSizeString(TotalFileSize);
	LB_RemainTime->Caption = IncSecond(0,(TotalFileSize - EndPos) / (EndPos / LastSecendCount) ).FormatString("HH:NN:SS");

}


void __fastcall TMainForm::ReadPerSecProce(){
	LB_ReadPerSec->Caption = MakeFileSizeString(ReadPerSec) + " / 초";
	ReadPerSec = 0;
}

void __fastcall TMainForm::GetMD5HashProce(int Index){
	TMyListItem *Item =(TMyListItem *)LV_ItemList->Items->Item[Index]->Data;
   //	DWORD dwStatus = 0;
	BOOL bResult ;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;

	hFile = CreateFileA(Item->FileName.c_str(),GENERIC_READ,FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (INVALID_HANDLE_VALUE == hFile)return ;

	if (!CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT))return ;

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)){
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return ;
	}

	ReadPerSecTimer->Enabled = true;
	TotalWorkerTimer->Enabled = true;

	try {
		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,&cbRead, NULL)){
			if (cbRead == 0)break;
			EndPos += cbRead;
			ReadPerSec += cbRead;
			if (!CryptHashData(hHash, rgbFile, cbRead, 0)){
//				dwStatus = GetLastError();
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return ;
			}
			Application->ProcessMessages();
		}

	} catch (Exception &E) { bResult = FALSE;	}

	if (!bResult){
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return ;
	}

	cbHash = MD5LEN;
	if(CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)){
		AnsiString stTemp = "";
		for (DWORD i = 0; i < cbHash; i++){
			Item->MD5Hash = Item->MD5Hash + stTemp.sprintf("%c%c", rgbDigits[rgbHash[i] >> 4],rgbDigits[rgbHash[i] & 0xf]);
		}
	}
	else Item->MD5Hash = "";

	Item->MD5Hash = Item->MD5Hash.UpperCase();
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);
	Item->Stat = 10;
	LV_ItemList->Items->Item[Index]->SubItems->Strings[ 1 ] = "완료";
	if (Item->MD5Hash != "")this->Shere_ini->WriteString(INI_SHEREED_NAME,Item->FileName,Item->MD5Hash);
//	ReadPerSecProce();
	TotalWorkProce();
	TotalWorkerTimer->Enabled = false;
	ReadPerSecTimer->Enabled = false;
}


void __fastcall TMainForm::AddListItem(AnsiString FileName){
	TMyListItem *LItem = new TMyListItem;
	LItem->FileName = FileName;
	LItem->FileSize = GetLocalFileSize(FileName);
	LItem->MD5Hash = this->Shere_ini->ReadString(INI_SHEREED_NAME,FileName,"");
	if(LItem->MD5Hash != "")LItem->Stat = 10;
	AnsiString Ext = ExtractFileExt(FileName);
	int idx;
	try {
		idx = this->SysIconList->IsInExtSystemIcon( Ext );
		if ( idx == 0 )idx = this->SysIconList->AddExtSystemIcon(Ext);
	} catch (...) {		}

	TListItem *Item = LV_ItemList->Items->Add();
	Item->ImageIndex = idx;

	Item->Caption = " "+ ExtractFileName(FileName);
	Item->SubItems->Add( MakeFileSizeString(LItem->FileSize));
	if(LItem->MD5Hash != "")Item->SubItems->Add("완료");
	else Item->SubItems->Add("");
	Item->Data = LItem;
}

void __fastcall TMainForm::Btn_ClearItemClick(TObject *Sender){
	LV_ItemList->Items->Clear();
}


