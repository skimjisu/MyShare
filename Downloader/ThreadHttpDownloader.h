#ifndef ThreadHttpDownloaderH
#define ThreadHttpDownloaderH


#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.DateUtils.hpp>

#include <Vcl.Dialogs.HPP>

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include <IdIOHandler.hpp>
#include <IdIOHandlerStream.hpp>
#include "Common.h"

enum DOWN_ERR_CODE {
	DOWN_ERR_URLERR,
	DOWN_ERR_DOWNDONE,
	DOWN_ERR_NOERROR,
};

enum MODE_DOWN_STAT {
	DOWN_STAT_NOT_DOWN,		// 다운 시작 안함
	DOWN_STAT_CONNING,		// 접속중
	DOWN_STAT_CONN_ERROR,	// 접속 오류
	DOWN_STAT_DOWNCALCEL,		// 다운 중지
	DOWN_STAT_DOWNING,		// 다운중
	DOWN_STAT_DOWNDONE,		//  다운 완료
	DOWN_STAT_DOWNERROR,	// 다운오류
	DOWN_STAT_FILE_ERROR,	// 파일 오류

};

static const char *DownStatStr[] = {
	"대기",		// 다운 시작 안함
	"연결중", 	// 접속중
	"연결오류", // 연결못함
	"중지",		// 다운 중지
	"다운중",	// 다운중
	"완료",		//  다운 완료
	"오류",		// 다운오류
	"파일오류",		// 파일오류

};

#define  DEFAULT_BLOCK_SIZE					5000000   // 5 메가


typedef void __fastcall (__closure *TThreadHttpDownloadEndEvent)(System::TObject* Sender, int Tag);
typedef void __fastcall (__closure *TThreadHttpDownloadWorkEvent)(System::TObject* Sender, int Tag,__int64 NowPos,__int64 TotalSize);
typedef void __fastcall (__closure *TThreadHttpDownloadStartEvent)(System::TObject* Sender, int Tag,__int64 StartPos,__int64 TotalSize);
typedef void __fastcall (__closure *TThreadHttpDownloadSpeedEvent)(System::TObject* Sender, int Tag,__int64 SpeedSize);
typedef void __fastcall (__closure *TThreadHttpDownloadRemainTimeEvent)(System::TObject* Sender, int Tag,AnsiString RemainTimeString);


class TThreadHttpDownload : public TThread{

public:
	__property TThreadHttpDownloadStartEvent OnStart = {read=FOnStart, write=FOnStart, default=NULL};
	__property TThreadHttpDownloadWorkEvent OnWork = {read=FOnWork, write=FOnWork, default=NULL};
	__property TThreadHttpDownloadEndEvent OnEnd = {read=FOnEnd, write=FOnEnd, default=NULL};
	__property TThreadHttpDownloadSpeedEvent OnSpeed = {read=FOnSpeed, write=FOnSpeed, default=NULL};
	__property TThreadHttpDownloadRemainTimeEvent OnRemainTime = {read=FOnRemainTime, write=FOnRemainTime, default=NULL};

	__property __int64 GetSendPerSecondSize = {read=SpeedCheckSize, nodefault};

	__property int Tag = {read=FTag, write=FTag, default=0};
	__property int BlockSize = {read=FBlockSize, write=FBlockSize, default=DEFAULT_BLOCK_SIZE};

	TIdHTTP *HTTPC;
	TTimer *SpeedCheckTimer;
	AnsiString NowDownLoadFilePath;
	AnsiString NowDownLoadFileURL;
	__int64 LastSecendCount,TotalSize,NowDownPos,LastPosition,NowSendSize,SpeedCheckSize;
	bool Downloading;

protected:
	void __fastcall Execute();
	TThreadHttpDownloadStartEvent FOnStart;
	TThreadHttpDownloadWorkEvent FOnWork;
	TThreadHttpDownloadEndEvent FOnEnd;
	TThreadHttpDownloadSpeedEvent FOnSpeed;
	TThreadHttpDownloadRemainTimeEvent FOnRemainTime;

	int FTag;
	int FBlockSize;

public:
	AnsiString LocalFileName;

	__fastcall TThreadHttpDownload();
	void __fastcall OnWorkProce();

	bool SaveStreamToFileEnd(AnsiString LocalFilePath, TCustomMemoryStream *SrcStream);

	__int8 __fastcall Download(AnsiString url, AnsiString LocalFileName);

	void __fastcall HTTPCWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	void __fastcall HTTPCWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	/*
	void __fastcall HTTPCWorkEnd(TObject *ASender, TWorkMode AWorkMode);
	*/

	void __fastcall SpeedCheckTimerTimer(TObject *Sender);
};



#endif
