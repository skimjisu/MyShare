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
	DOWN_STAT_NOT_DOWN,		// �ٿ� ���� ����
	DOWN_STAT_CONNING,		// ������
	DOWN_STAT_CONN_ERROR,	// ���� ����
	DOWN_STAT_DOWNCALCEL,		// �ٿ� ����
	DOWN_STAT_DOWNING,		// �ٿ���
	DOWN_STAT_DOWNDONE,		//  �ٿ� �Ϸ�
	DOWN_STAT_DOWNERROR,	// �ٿ����
	DOWN_STAT_FILE_ERROR,	// ���� ����

};

static const char *DownStatStr[] = {
	"���",		// �ٿ� ���� ����
	"������", 	// ������
	"�������", // �������
	"����",		// �ٿ� ����
	"�ٿ���",	// �ٿ���
	"�Ϸ�",		//  �ٿ� �Ϸ�
	"����",		// �ٿ����
	"���Ͽ���",		// ���Ͽ���

};

#define  DEFAULT_BLOCK_SIZE					5000000   // 5 �ް�


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
