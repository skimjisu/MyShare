#ifndef fmMainH
#define fmMainH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>

#include <System.StrUtils.hpp>
#include <Web.HTTPApp.hpp>

#include "..\SystemIconProce.h"
#include "..\ThreadHttpDownloader.h"
#include "..\Common.h"
#include <Vcl.Buttons.hpp>

// enum MODE_DOWN_STAT {
// DOWN_STAT_NOT_DOWN,		// 다운 시작 안함
// DOWN_STAT_CONNING,		// 접속중
// DOWN_STAT_CONN_ERROR,	// 접속 오류
// DOWN_STAT_DOWNCALCEL,		// 다운 중지
// DOWN_STAT_DOWNING,		// 다운중
// DOWN_STAT_DOWNDONE,		//  다운 완료
// DOWN_STAT_DOWNERROR,	// 다운오류
// DOWN_STAT_FILE_ERROR,	// 파일 오류
//
// };
//
// static const char *DownStatStr[] = {
// "대기",		// 다운 시작 안함
// "연결중", 	// 접속중
// "연결오류", // 연결못함
// "중지",		// 다운 중지
// "다운중",	// 다운중
// "완료",		//  다운 완료
// "오류",		// 다운오류
// "파일오류",		// 파일오류
//
// };

class TShereNameListItem : public TObject {

public:
	AnsiString Name, ShereNumber;
};

class TShereItem : public TObject {
public:
	AnsiString FileName, MD5, Name, ShereNumber, DownPath, url;
	__int64 FileSize;
	int IconIdx;
	__int8 DownStat;
	int ListItemIndex;

	__fastcall TShereItem();
	__fastcall TShereItem(AnsiString Name, AnsiString FileName, AnsiString MD5, __int64 FileSize);
};

__fastcall TShereItem::TShereItem() {
	this->Name = "";
	this->FileName = "";
	this->MD5 = "";
	this->FileSize = 0;
	this->DownStat = 0;
}

__fastcall TShereItem::TShereItem(AnsiString Name, AnsiString FileName, AnsiString MD5, __int64 FileSize) {
	this->Name = Name;
	this->FileName = FileName;
	this->MD5 = MD5;
	this->FileSize = FileSize;
	this->DownStat = 0;
}

class TMainForm : public TForm {
__published: // IDE-managed Components

	TStatusBar *StatusBar1;
	TPanel *Panel5;
	TLabel *Label1;
	TEdit *ED_Address;
	TEdit *ED_Port;
	TButton *Btn_Connect;
	TTreeView *TV_ShereList;
	TIdHTTP *HTTPC;
	TImageList *IconRes;
	TPopupMenu *PopupMenu2;
	TMenuItem *N5;
	TMenuItem *N7;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TMenuItem *N4;
	TListView *LV_DownList;
	TEdit *Edit1;
	TPanel *Panel1;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TSpeedButton *ItemDownload;
	TSpeedButton *LV_DownList_Del;
	TSpeedButton *SpeedButton2;
	TGroupBox *GroupBox5;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TSpeedButton *SpeedButton10;
	TSpeedButton *SpeedButton9;
	TMemo *Memo1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;

	void __fastcall Btn_ConnectClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TV_ShereListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TV_ShereListChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ShereDownClick(TObject *Sender);
	void __fastcall ShereItemDownClick(TObject *Sender);
	void __fastcall LV_DownListCustomDrawItem(TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
		bool &DefaultDraw);
	void __fastcall LV_DownListClick(TObject *Sender);
	void __fastcall LV_DownListSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall LV_DownList_DelClick(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall ItemDownloadClick(TObject *Sender);
	void __fastcall LV_DownListDrawItem(TCustomListView *Sender, TListItem *Item, TRect &Rect, TOwnerDrawState State);
	void __fastcall SpeedButton10Click(TObject *Sender);

private: // User declarations

	TExtIconList *SysIconList;

public: // User declarations

	__fastcall TMainForm(TComponent* Owner);
	TTreeNode * __fastcall AddRootNode(TTreeView *GroupList, AnsiString Name, TShereNameListItem *ItemObj);
	void __fastcall AddSubNode(TTreeView *GroupList, TTreeNode *RootNode, AnsiString FileName, TObject *Item);
	void __fastcall AddDownloadItem(TShereItem *DownItem);
};

extern PACKAGE TMainForm *MainForm;

#endif
