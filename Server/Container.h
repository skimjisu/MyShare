#ifndef ContainerH
#define ContainerH

#include <System.Classes.hpp>
#include <System.Contnrs.hpp>
#include <Vcl.Controls.hpp>
#include <System.StrUtils.hpp>

#include <Vcl.Dialogs.hpp>

#include "..\Common.h"

// body
#define	SHERE_LIST_AERE_TMPL		"[SHERE_LIST_AERE]"

// shere list
#define	SHERE_NAME_TMPL				"[SHERE_NAME]"
#define	SHERE_ITEM_LIST_TMPL		"[SHERE_ITEM_LIST]"

// shere item
#define	SHERE_ITEM_ADDRESS_TMPL		"[SHERE_ITEM_ADDRESS]"
#define	SHERE_ITEM_NAME_TMPL		"[SHERE_ITEM_NAME]"
#define	SHERE_ITEM_MD5_TMPL			"[SHERE_ITEM_MD5]"
#define	SHERE_ITEM_SIZE_TMPL		"[SHERE_ITEM_SIZE]"
#define	SHERE_NUMBER_TMPL			"[SHERE_NUMBER]"

class TShereItem : public TObject {
public:
	AnsiString FileName,MD5,Name;
	__int64 FileSize;
	int FItemIndex;

public:
	__fastcall TShereItem();
	__fastcall TShereItem(AnsiString Name,AnsiString FileName,AnsiString MD5,__int64 FileSize);
	__property int ItemIndex = {read=FItemIndex,write=FItemIndex, default=0};

};

class TShereListItem : public TObject {

private:
	AnsiString FName;
	TList *FItems;
	int FTag;
	int FShereNumber;


private:
	int __fastcall GetItemCount();

public:

	__fastcall TShereListItem();
	__fastcall TShereListItem(AnsiString Name);
	void __fastcall Add(TShereItem *Item);
	TShereItem * __fastcall FindMD5(AnsiString MD5);
	void __fastcall Delete(TShereItem *Item);
	void __fastcall FindAndDeleteMD5(AnsiString MD5);
	AnsiString __fastcall GetAllListToString();
	AnsiString __fastcall GetAllListToHTML(AnsiString item_tmpl);


	__property AnsiString Name = {read=FName, write=FName, default=NULL};
	__property TList* Items = {read=FItems, write=FItems, default=NULL};
	__property int Tag = {read=FTag, write=FTag, default=0};
	__property int ShereNumber = {read=FShereNumber, write=FShereNumber, default=0};

	__property int Count = {read=GetItemCount, default=0};

};


class TContainer : public TObject {

private:
	TList *ShereList;
	TList *SendingList;
	__int64 TotalShereFileSize;
	int TotalShereCount;
	int ShereListCount;

public:

	__fastcall TContainer();
	int __fastcall GetShereNumber();
	void __fastcall AddItem(AnsiString ShereName, TShereItem *Item);
	void __fastcall DeleteItem(AnsiString ShereName, TShereItem *Item);
	TShereListItem * __fastcall FineShereName(AnsiString ShereName);
	TShereItem * __fastcall FindShereItem(int ShereNumber,AnsiString MD5);

	AnsiString __fastcall GetShereListToString();
	AnsiString __fastcall GetShereListItemToString(int ShereNumber);
	void __fastcall DeleteShereListFromName(AnsiString ShereName);
	AnsiString __fastcall MakeShereListToHTML(AnsiString body_tmpl,AnsiString list_tmpl,AnsiString item_tmpl);

	void __fastcall ClearShereList();
	void __fastcall ClearSendingList();
};



#endif
