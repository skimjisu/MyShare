#ifndef fmMainH
#define fmMainH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <System.StrUtils.hpp>

#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomHTTPServer.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdHTTPServer.hpp>
#include <IdScheduler.hpp>
#include <IdSchedulerOfThread.hpp>
#include <IdSchedulerOfThreadDefault.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>

#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include <IniFiles.hpp>

#include <Web.HTTPApp.hpp>

#include "fmMakeDir.h"

#include "..\SystemIconProce.h"
#include "..\Common.h"
#include "Container.h"

#include "ServerProce.h"
#include <IdContext.hpp>
#include <System.ImageList.hpp>

#define		INI_SHERE_LIST_NAME			"Shere_Name_List"
#define		INI_FILE_HASH_DONE_NAME		"Hash_done_List"

#define MIN_SHERE_FILE_SIZE					10				// �Ѱ��� ������ ���� �ּ� ������
#define MAX_SHERE_FILE_SIZE					68719475199LL	 	// �Ѱ��� ������ ���� �ִ� ������ 68719475199 : 68�Ⱑ

#define BUFSIZE 4096
#define MD5LEN  16

char rgbDigits[] = "0123456789abcdef";

class TMainForm : public TForm {
__published: // IDE-managed Components

	TPageControl *PageControl1;
	TIdHTTPServer *HTTPS;
	TImageList *IconRes;
	TIdSchedulerOfThreadDefault *IdSchedulerOfThreadDefault1;
	TOpenDialog *OpenDialog1;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TMenuItem *N4;
	TPopupMenu *PopupMenu2;
	TMenuItem *N5;
	TMenuItem *N7;
	TTabSheet *TabSheet1;
	TTimer *WorkTimer;
	TPanel *Panel1;
	TLabel *Label2;
	TPanel *Panel2;
	TButton *Btn_DeleteDir;
	TButton *Btn_MakeDir;
	TPanel *Panel4;
	TButton *Btn_Shere;
	TPanel *Panel3;
	TLabel *LB_WorkName;
	TImage *Image1;
	TPanel *PN_WorkProce;
	TLabel *Label3;
	TLabel *LB_Percent;
	TLabel *Label5;
	TLabel *LB_NowFileName;
	TProgressBar *PB_Total;
	TButton *Btn_HashCencel;
	TStatusBar *StatusBar1;
	TTabSheet *TabSheet2;
	TMemo *Memo1;
	TTreeView *TV_ShereList;
	TTabSheet *��Ȳ��;

	void __fastcall HTTPSCommandGet(TIdContext *AContext, TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo);
	void __fastcall FormCreate(TObject *Sender);


	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Btn_MakeDirClick(TObject *Sender);
	void __fastcall TV_ShereListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TV_ShereListChange(TObject *Sender, TTreeNode *Node);
	void __fastcall TV_ShereListEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);
	void __fastcall Btn_DeleteDirClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall TV_ShereListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Btn_ShereClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall WorkTimerTimer(TObject *Sender);
	void __fastcall TotalWorkProce();
	void __fastcall Btn_HashCencelClick(TObject *Sender);

private: // User declarations

	TContainer *Container;

	TExtIconList *SysIconList;

	TIniFile *Shere_ini;

	bool IS_Hashed, IS_Shering, IS_HashCencel;

	__int64 TotalFileSize, EndPos;

public: // User declarations

	__fastcall TMainForm(TComponent* Owner);
	void __fastcall initializeIcons();
	void __fastcall updateShareBtcp();
	void __fastcall Load_INI();
	void __fastcall handleItem(TStringList *List, TStringList *ItemList, TTreeNode *rootNode, int i, int j);
	void __fastcall deleteFileKeys(TStringList *List, TStringList *ItemList, int i, int j);
	TTreeNode * __fastcall AddRootNode(TTreeView *GroupList, AnsiString Name, bool WriteINI);
	void __fastcall WorkForWait(bool wait);
	void __fastcall AddSubNode(TTreeView *GroupList, TTreeNode *RootNode, AnsiString FileName, TObject *Item, __int64 FileSize,
		bool WriteINI);

	bool __fastcall IsInItem(TTreeNode *Node, AnsiString Name);
	void __fastcall HandleNodeSelection(TTreeView *treeView, TTreeNode *node);
	void __fastcall DeleteSubNodes(TTreeNode *RootNode);
	int __fastcall GetCountOfSelectedItems(TTreeView *treeView);
	void __fastcall AddShereFile(TTreeNode *Node);
	void __fastcall ProcessFile(const AnsiString &filePath, TTreeNode *node);
   //	int __fastcall GetCountOfSelectedItems(TTreeView *treeView);
	void __fastcall DeleteShere(TObject *Sender);
	AnsiString __fastcall GetMD5HashProce(AnsiString FileName);
	bool __fastcall HashingProce();
	void __fastcall tartFileCheck();
	void __fastcall StopFileCheck();
	void __fastcall StartSharing();
	void __fastcall StopSharing();
	void __fastcall ProcessRegularRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo);
	void __fastcall ProcessDownloaderRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo);
    void __fastcall LogHTTPRequest(TIdHTTPRequestInfo *ARequestInfo);

};

extern PACKAGE TMainForm *MainForm;

#endif