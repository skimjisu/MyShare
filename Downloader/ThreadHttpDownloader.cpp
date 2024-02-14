#pragma hdrstop
#include "ThreadHttpDownloader.h"
#pragma package(smart_init)


__fastcall TThreadHttpDownload::TThreadHttpDownload(): TThread(true){
	HTTPC = new TIdHTTP;
	SpeedCheckTimer = new TTimer(NULL);
	SpeedCheckTimer->Enabled = false;
	SpeedCheckTimer->Interval = 1000;
	SpeedCheckTimer->OnTimer = SpeedCheckTimerTimer;
	HTTPC->Request->UserAgent = DOWNLOADER_USER_AGENT_STRING;
	HTTPC->OnWork 		= HTTPCWork;
	HTTPC->OnWorkBegin 	= HTTPCWorkBegin;
	//HTTPC->OnWorkEnd 	= HTTPCWorkEnd;

	Priority = tpIdle;
//	Priority = tpHighest;

	FreeOnTerminate = true;
	Downloading = false;
	FBlockSize = DEFAULT_BLOCK_SIZE;

}


__int8 __fastcall TThreadHttpDownload::Download(AnsiString url, AnsiString LocalFileName){
	NowDownLoadFilePath = LocalFileName;
	NowDownLoadFileURL = url;
	LastSecendCount = 0;

	this->LastPosition = 0;
	this->NowSendSize = 0;

	HTTPC->Head(url);
	TotalSize = HTTPC->Response->ContentLength;
	if(TotalSize == -1  || TotalSize == 0)return DOWN_ERR_URLERR;
	if( GetLocalFileSize(NowDownLoadFilePath) >= TotalSize )return DOWN_ERR_DOWNDONE;
	if(!DirectoryExists(ExtractFilePath( NowDownLoadFilePath  ))){
		try { ForceDirectories(ExtractFilePath( NowDownLoadFilePath )); } catch (...) {	 }
	}

	Resume();
	return DOWN_ERR_NOERROR;
}

void __fastcall TThreadHttpDownload::Execute(){
	TMemoryStream *DownStream = new TMemoryStream;
	__int64 SendSize;
	if(FileExists(NowDownLoadFilePath))NowDownPos = GetLocalFileSize(NowDownLoadFilePath);
	else NowDownPos = 0;
	SpeedCheckTimer->Enabled = true;
	Downloading = true;
	if( FOnStart != NULL )FOnStart(this,this->Tag,NowDownPos,TotalSize);

	while(true){
		DownStream->Clear();
		SendSize = 0;
		if( (NowDownPos + FBlockSize) >= TotalSize )HTTPC->Request->Range = IntToStr(NowDownPos) + "-";
		else HTTPC->Request->Range = IntToStr(NowDownPos) + "-" + IntToStr(NowDownPos + FBlockSize);
		try {
			HTTPC->Get(NowDownLoadFileURL,DownStream);
			SendSize = DownStream->Size;
//		   	SpeedCheckSize = SpeedCheckSize + SendSize;
		} catch (Exception &E) {
			SendSize = 0 ;
		}

		if (SendSize > 0)SaveStreamToFileEnd(NowDownLoadFilePath,DownStream);
		else break;

//		if( FOnWork != NULL )FOnWork(this,this->Tag,NowDownPos,TotalSize);
		Synchronize(&OnWorkProce);

		if( (NowDownPos + SendSize) >= TotalSize ){
			NowDownPos = NowDownPos + SendSize;
			break;
		}
		WaitForSingleObject((void*)this->Handle,200);
		NowDownPos = NowDownPos + SendSize;

	}

	if( FOnWork != NULL )FOnWork(this,this->Tag,NowDownPos,TotalSize);

	delete DownStream;
	if( FOnEnd != NULL )FOnEnd(this,this->Tag);
	this->Suspend();
	SpeedCheckTimer->Enabled = false;
	Downloading = false;

	delete this;
}


void __fastcall TThreadHttpDownload::OnWorkProce(){
	if( FOnWork != NULL )FOnWork(this,this->Tag,NowDownPos,TotalSize);
	//Application->ProcessMessages();
}


void __fastcall TThreadHttpDownload::HTTPCWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax){
	this->LastPosition = 0;
	this->NowSendSize = 0;

}

void __fastcall TThreadHttpDownload::HTTPCWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount){
	NowSendSize = AWorkCount - LastPosition;
	LastPosition = AWorkCount;
	SpeedCheckSize = SpeedCheckSize + NowSendSize;
	//Application->ProcessMessages();
}

/*
void __fastcall TThreadHttpDownload::HTTPCWorkEnd(TObject *ASender, TWorkMode AWorkMode){

}
*/

void __fastcall TThreadHttpDownload::SpeedCheckTimerTimer(TObject *Sender){
	LastSecendCount ++;
	if(NowDownPos > 0 && FOnRemainTime != NULL)FOnRemainTime(this,this->Tag, IncSecond(0,(TotalSize - NowDownPos) / (NowDownPos / LastSecendCount) ).FormatString("HH:NN:SS"));
	if( FOnSpeed != NULL)FOnSpeed(this,this->Tag,SpeedCheckSize);
	SpeedCheckSize = 0;
}

bool TThreadHttpDownload::SaveStreamToFileEnd(AnsiString LocalFilePath, TCustomMemoryStream *SrcStream){
	TFileStream *SaveFile;
	try {
		if ( FileExists(LocalFilePath) )SaveFile = new TFileStream(LocalFilePath, fmOpenReadWrite | fmShareDenyWrite);
		else SaveFile = new TFileStream(LocalFilePath, fmCreate | fmOpenReadWrite | fmShareDenyWrite);

		SaveFile->Position =  SaveFile->Size;
		SrcStream->Position = 0;
		SaveFile->CopyFrom(SrcStream, SrcStream->Size);
		delete SaveFile;
	} catch (...) { delete SaveFile; return false; }
	return true;
}




