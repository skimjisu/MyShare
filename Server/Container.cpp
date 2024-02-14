#pragma hdrstop
#include "Container.h"
#pragma package(smart_init)


__fastcall TShereItem::TShereItem(){
	this->Name = "";
	this->FileName = "";
	this->MD5 = "";
	this->FileSize = 0;
}

__fastcall TShereItem::TShereItem(AnsiString Name,AnsiString FileName,AnsiString MD5,__int64 FileSize){
	this->Name = Name;
	this->FileName = FileName;
	this->MD5 = MD5;
	this->FileSize = FileSize;
}


__fastcall TShereListItem::TShereListItem(){
	this->FItems = new TList;
	this->Name = "";
}

__fastcall TShereListItem::TShereListItem(AnsiString Name){
	this->FItems = new TList;
	this->Name = Name;
}

int __fastcall TShereListItem::GetItemCount(){
	if(this->Items != NULL)return this->Items->Count;
	else return 0;
}

void __fastcall TShereListItem::Add(TShereItem *Item){
	Item->ItemIndex = this->FItems->Add(Item);
}

TShereItem * __fastcall TShereListItem::FindMD5(AnsiString MD5){
	TShereItem *Item;
	for (int i = 0; i < this->Items->Count; i++) {
		Item = (TShereItem *)this->Items->Items[i];
		if(Item->MD5 == MD5)return Item;
	}
	return NULL;
}

AnsiString __fastcall TShereListItem::GetAllListToString(){
	TShereItem *Item;
	AnsiString stTemp = "";
	if(  this->Items->Count == NULL ||  this->Items->Count <= 0 )return "";
	for (int i = 0; i < this->Items->Count; i++) {
		Item = (TShereItem *)this->Items->Items[i];
		stTemp =  stTemp + 	ExtractFileName(Item->FileName) + TOKEN2 + Item->MD5 + TOKEN2 + IntToStr(Item->FileSize) +  TOKEN;
	}
	return stTemp;
}


AnsiString __fastcall TShereListItem::GetAllListToHTML(AnsiString item_tmpl){
	TShereItem *Item;
	AnsiString stTemp = "",stTemp2 = "";
	if(  this->Items->Count == NULL ||  this->Items->Count <= 0 )return "";
	for (int i = 0; i < this->Items->Count; i++) {
		Item = (TShereItem *)this->Items->Items[i];
		stTemp2 = AnsiReplaceStr( item_tmpl ,SHERE_ITEM_ADDRESS_TMPL , "#" );
		stTemp2 = AnsiReplaceStr( stTemp2 ,SHERE_ITEM_NAME_TMPL , ExtractFileName(Item->FileName) );
		stTemp2 = AnsiReplaceStr( stTemp2 ,SHERE_ITEM_MD5_TMPL , Item->MD5 );
		stTemp2 = AnsiReplaceStr( stTemp2 ,SHERE_ITEM_SIZE_TMPL , MakeFileSizeString(Item->FileSize) );
		stTemp2 = AnsiReplaceStr( stTemp2 ,SHERE_NUMBER_TMPL , IntToStr(this->ShereNumber) );

		stTemp =  stTemp + stTemp2 + "\n";
	}
	return stTemp;
}


void __fastcall TShereListItem::Delete(TShereItem *Item){
	this->Items->Delete(Item->ItemIndex);
}

void __fastcall TShereListItem::FindAndDeleteMD5(AnsiString MD5){
	TShereItem *Item = this->FindMD5(MD5);
	if(Item != NULL)this->Delete(Item);
}

__fastcall TContainer::TContainer(){
	this->ShereList = new TList;
	this->SendingList = new TList;
	this->TotalShereFileSize = 0;
	this->TotalShereCount = 0;
	this->ShereListCount = 0;
}

int __fastcall TContainer::GetShereNumber(){
	this->ShereListCount ++;
	return this->ShereListCount;
}

TShereListItem * __fastcall TContainer::FineShereName(AnsiString ShereName){
	TShereListItem *Item;
	for (int i = 0; i < this->ShereList->Count; i++) {
		Item = (TShereListItem *)this->ShereList->Items[i];
		if(	Item->Name == ShereName)return Item;
	}
	return NULL;
}

TShereItem * __fastcall TContainer::FindShereItem(int ShereNumber,AnsiString MD5){
	TShereListItem *ShereList;
	for (int i = 0; i < this->ShereList->Count; i++) {
		ShereList = (TShereListItem *)this->ShereList->Items[i];
		if(	ShereList->ShereNumber == ShereNumber){
			TShereItem *Item = ShereList->FindMD5(MD5);
			if(Item != NULL)return Item;
			else return NULL;
		}
	}
	return NULL;
}

void __fastcall TContainer::AddItem(AnsiString ShereName, TShereItem *Item){
	TShereListItem *ShereListItem = this->FineShereName(ShereName);

	if(ShereListItem != NULL){
		ShereListItem->Add(Item);
	}
	else {
		ShereListItem = NULL;
		ShereListItem = new TShereListItem(ShereName);
		ShereListItem->ShereNumber = this->GetShereNumber();
		ShereListItem->Add(Item);
		this->ShereList->Add(ShereListItem);

	}

}

void __fastcall TContainer::DeleteItem(AnsiString ShereName, TShereItem *Item){
	TShereListItem *ShereListItem = this->FineShereName(ShereName);
	if(ShereListItem != NULL)ShereListItem->Delete(Item);
}

void __fastcall TContainer::DeleteShereListFromName(AnsiString ShereName){
	TShereListItem *ItemList;
	for (int i = 0; i < this->ShereList->Count; i++) {
		ItemList = (TShereListItem *)this->ShereList->Items[i];
		if( ItemList->Name == ShereName){
			delete ItemList;
			this->ShereList->Delete(i);
			break;
		}
	}
}


AnsiString __fastcall TContainer::GetShereListToString(){
	TShereListItem *Item;
	AnsiString stTemp = "";
	for (int i = 0; i < this->ShereList->Count; i++) {
		Item = (TShereListItem *)this->ShereList->Items[i];
		stTemp =  stTemp + 	IntToStr(Item->ShereNumber) + TOKEN2 + Item->Name + TOKEN;
	}
	return stTemp;
}

AnsiString __fastcall TContainer::GetShereListItemToString(int ShereNumber){
	TShereListItem *ItemList;
	for (int i = 0; i < this->ShereList->Count; i++) {
		ItemList = (TShereListItem *)this->ShereList->Items[i];
		if( ItemList->ShereNumber == ShereNumber){
			return ItemList->GetAllListToString();
		}
	}
	return "";
}

AnsiString __fastcall TContainer::MakeShereListToHTML(AnsiString body_tmpl,AnsiString list_tmpl,AnsiString item_tmpl){

	TShereListItem *ItemList;
	AnsiString stTemp = "",stTemp2="",stTemp3 = "";
	for (int i = 0; i < this->ShereList->Count; i++) {
		ItemList = (TShereListItem *)this->ShereList->Items[i];
		stTemp = ItemList->GetAllListToHTML(item_tmpl);
		stTemp2 = AnsiReplaceStr( list_tmpl ,SHERE_NAME_TMPL , ItemList->Name );
		stTemp2 = AnsiReplaceStr( stTemp2 ,SHERE_ITEM_LIST_TMPL ,stTemp );
		stTemp3 = stTemp3 + stTemp2;
	}
	return AnsiReplaceStr( body_tmpl ,SHERE_LIST_AERE_TMPL , stTemp3 );
}

void __fastcall TContainer::ClearShereList(){
	TShereListItem *Item;
	for ( int i = this->ShereList->Count - 1; i >= 0; --i ){
		Item = (TShereListItem *)this->ShereList->Items[i];
		Item->Items->Clear();
		delete Item;
	}
}


void __fastcall TContainer::ClearSendingList(){
	this->SendingList->Clear();
}



