#pragma hdrstop
#include "SystemIconProce.h"

#pragma package(smart_init)


int __fastcall TExtIconList::AddExtSystemIcon(AnsiString Ext){
	SHFILEINFOA IconInfo;
	SHGetFileInfoA( AnsiString("." + Ext).c_str() , 0, &IconInfo, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON );
	TIcon *icon = new TIcon;

	icon->Handle = IconInfo.hIcon;
	int i = IconList->AddIcon(icon);
	delete icon;
	TIconItem *Item = new TIconItem;
	Item->Ext = Ext;
	Item->IconIdex = i;
	IconInfoList->Add(Item);
	return i;
}

int __fastcall TExtIconList::IsInExtSystemIcon(AnsiString FileExt){
	for (int i = 0; i < IconInfoList->Count; i++){
		TIconItem *Item = (TIconItem *)IconInfoList->Items[i];
		if (Item->Ext == FileExt){
			return Item->IconIdex;
		}
	}
	return 0;
}


int __fastcall TExtIconList::AddCustomSystemIcon(AnsiString Ext,TIcon *AddIcon){
	TIcon *icon = new TIcon;
	icon->Handle = AddIcon->Handle;
	int i = IconList->AddIcon(icon);
	delete icon;
	TIconItem *Item = new TIconItem;
	Item->Ext = Ext;
	Item->IconIdex = i;
	IconInfoList->Add(Item);
	return i;
}