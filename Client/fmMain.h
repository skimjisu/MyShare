#ifndef fmMainH
#define fmMainH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <System.StrUtils.hpp>
#include <IniFiles.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.DateUtils.hpp>

#include "Common.h"
#include "SystemIconProce.h"

#define	INI_SHEREED_NAME					"ÇØ½Ì¿Ï·á"
#define BUFSIZE 4096
#define MD5LEN  16

char rgbDigits[] = "0123456789abcdef";

class TMyListItem : public TObject {
public:
	AnsiString FileName;
	__int64 FileSize;
	AnsiString MD5Hash;
	int IconIdx;
	__int8 Stat;
	__fastcall TMyListItem(){
		Stat = 0;
	}
};


class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel3;
	TListView *LV_ItemList;
	TProgressBar *PB_WorkerProce;
	TOpenDialog *Dlg_ChoiceFile;
	TTimer *FileWorkerTimer;
	TTimer *ReadPerSecTimer;
	TTimer *TotalWorkerTimer;
	TLabel *LB_ReadPerSec;
	TLabel *LB_TotalProce;
	TLabel *LB_TotalPercent;
	TLabel *LB_RemainTime;
	TLabel *LB_TotalSize;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *LB_NowFile;
	TPanel *Panel2;
	TPanel *Panel4;
	TButton *Btn_Start;
	TButton *Btn_ClearItem;
	TButton *Btn_ChoiceFile;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Btn_ChoiceFileClick(TObject *Sender);
	void __fastcall Btn_StartClick(TObject *Sender);
	void __fastcall FileWorkerTimerTimer(TObject *Sender);
	void __fastcall ReadPerSecTimerTimer(TObject *Sender);
	void __fastcall TotalWorkerTimerTimer(TObject *Sender);
	void __fastcall Btn_ClearItemClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	TIniFile *Shere_ini;
	TExtIconList *SysIconList;


public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	void __fastcall AddListItem(AnsiString FileName);
	void __fastcall GetMD5HashProce(int Index);
	void __fastcall ReadPerSecProce();
	void __fastcall TotalWorkProce();


};

extern PACKAGE TMainForm *MainForm;

#endif
