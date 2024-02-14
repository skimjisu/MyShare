#include <vcl.h>
#pragma hdrstop

#include "fmMain.h"
#include "ThreadHttpDownloader.h"
#include "fmSet.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;
TThreadHttpDownload * FileDW;

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {

	FileDW = new TThreadHttpDownload();
	FileDW->NowDownLoadFileURL = "http://114.203.106.114:5425/LG_Recovery.iso"; // 다운로드 주소
	FileDW->NowDownLoadFilePath = "c:/"; // 다운로드 위치

}

void __fastcall TMainForm::FormCreate(TObject *Sender) {
	SysIconList = new TExtIconList;
	TV_ShereList->Images = SysIconList->IconList;

	LV_DownList->SmallImages = SysIconList->IconList;

	// TV_ShereList->Images = IconRes;

	TIcon *icon = new TIcon;
	IconRes->GetIcon(0, icon);
	SysIconList->AddCustomSystemIcon("__Directory", icon);
	delete icon;

	icon = new TIcon;
	IconRes->GetIcon(1, icon);
	SysIconList->AddCustomSystemIcon("__Directory_Open", icon);
	delete icon;

	HTTPC->Request->UserAgent = DOWNLOADER_USER_AGENT_STRING;

	TListColumn *Item = LV_DownList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "                               파            일            명";
	Item->Width = 320;

	Item = LV_DownList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "크 기";
	Item->Width = 180;

	Item = LV_DownList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "속  도";
	Item->Width = 95;

	Item = LV_DownList->Columns->Add();
	Item->Alignment = taCenter;
	Item->Caption = "Status";
	Item->Width = 70;
}

void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	delete SysIconList;
}

void __fastcall TMainForm::Btn_ConnectClick(TObject *Sender) {

	if (ED_Address->Text.Pos("/") != 0 || ED_Address->Text.Pos(":") != 0) {
		ShowMessage("잘못된 주소 입니다.");
		return;
	}

	if (ED_Port->Text.ToInt() <= 1024) {
		ShowMessage("포트는 1024 번 이후로 설정 하여야 합니다.");
		return;
	}

	AnsiString Url = "http://" + ED_Address->Text + ":" + ED_Port->Text + "/shere_list";
	AnsiString stTemp;

	try {
		stTemp = HTTPC->Get(Url);
	}
	catch (...) {
		ShowMessage("주소나 포트번호가 잘못 되었습니다");
		return;
	}

	stTemp = stTemp.Trim();
	if (stTemp == "") {
		ShowMessage("주소나 포트번호가 잘못 되었습니다");
		return;

	}

	TStringList *List = new TStringList;
	List->Text = AnsiReplaceStr(stTemp, TOKEN, "\n");
	List->Text = List->Text.Trim();

	if (List->Count < 1) {
		ShowMessage("주소나 포트번호가 잘못 되었습니다");
		return;
	}

	TV_ShereList->Items->Clear();

	TStringList *ListItem = new TStringList;

	for (int i = 0; i < List->Count; i++) {
		ListItem->Clear();
		ListItem->Text = AnsiReplaceStr(List->Strings[i], TOKEN2, "\n");
		TShereNameListItem *Item = new TShereNameListItem;
		Item->Name = ListItem->Strings[1];
		Item->ShereNumber = ListItem->Strings[0];
		TTreeNode *RootNode = AddRootNode(TV_ShereList, ListItem->Strings[1], Item);
	}
	ListItem->Clear();

	AnsiString GetUrl = "";
	for (int i = 0; i < TV_ShereList->Items->Count; i++) {
		TTreeNode *RootNode = TV_ShereList->Items->Item[i];
		if (RootNode->Level == 0) {
			TShereNameListItem *ShereListItem = (TShereNameListItem*)RootNode->Data;
			GetUrl = "http://" + ED_Address->Text + ":" + ED_Port->Text + "/shere_item_list?" + ShereListItem->ShereNumber;
			try {
				stTemp = HTTPC->Get(GetUrl);
			}
			catch (...) {
			}
			List->Text = AnsiReplaceStr(stTemp, TOKEN, "\n");
			List->Text = List->Text.Trim();
			for (int j = 0; j < List->Count; j++) {
				ListItem->Text = AnsiReplaceStr(List->Strings[j], TOKEN2, "\n");
				TShereItem *Item = new TShereItem(ShereListItem->Name, ListItem->Strings[0], ListItem->Strings[1],
					StrToInt64(ListItem->Strings[2]));
				AddSubNode(TV_ShereList, RootNode, ListItem->Strings[0], Item);
			}
		}
	}
	delete ListItem;
	delete List;

}

TTreeNode * __fastcall TMainForm::AddRootNode(TTreeView *GroupList, AnsiString Name, TShereNameListItem *ItemObj) {
	// TTreeNode *Item = GroupList->Items->Add(NULL,Name);
	TTreeNode *Item = GroupList->Items->AddObject(NULL, Name, ItemObj);
	Item->ImageIndex = this->SysIconList->IsInExtSystemIcon("__Directory");
	Item->SelectedIndex = this->SysIconList->IsInExtSystemIcon("__Directory_Open");
	return Item;
}

void __fastcall TMainForm::AddSubNode(TTreeView *GroupList, TTreeNode *RootNode, AnsiString FileName, TObject *Item) {
	if (RootNode != NULL) {
		TTreeNode *SubNode = GroupList->Items->AddChildObject(RootNode, FileName, Item);
		AnsiString Ext = ExtractFileExt(FileName);
		int i = this->SysIconList->IsInExtSystemIcon(Ext);
		if (i != 0) {
			SubNode->ImageIndex = i;
			SubNode->SelectedIndex = i;
		}
		else {
			i = this->SysIconList->AddExtSystemIcon(Ext);
			SubNode->ImageIndex = i;
			SubNode->SelectedIndex = i;
		}
	}

}

void __fastcall TMainForm::TV_ShereListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TTreeView *p = (TTreeView*)Sender;
	TTreeNode *Node = p->GetNodeAt(X, Y);
	if (Node == NULL)
		return;
	Node->Selected = true;
	if (Button != mbRight)
		return;

	if (Node->Level == 0) {
		N1->Caption = Node->Text + " 항목 전체 다운로드 추가...";
		this->PopupMenu1->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}

	else if (p->SelectionCount >= 1) {
		int SCount = 0;
		for (unsigned int i = 0; i < p->SelectionCount; i++) {
			if (p->Selections[i]->Level == 1) {
				SCount++;
			}
		}
		N5->Caption = IntToStr(SCount) + " 개 항목 다운로드 추가...";
		this->PopupMenu2->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}
}

void __fastcall TMainForm::TV_ShereListChange(TObject *Sender, TTreeNode *Node) {
	if (Node->Level == 0)
		TV_ShereList->MultiSelect = false;
	else
		TV_ShereList->MultiSelect = true;
}

void __fastcall TMainForm::ShereDownClick(TObject *Sender) {
	AnsiString Dir = BrowseFolder(this->Handle, "다운로드 폴더를 선택해 주세요", "");
	if (Dir == "")
		return;
	Set_Form->Dir_ED->Text = Dir;
	TTreeNode *RootNode = TV_ShereList->Selected;
	if (RootNode->Level != 0)
		return;
	TTreeNode *Node;
	TShereNameListItem *RootItem;
	TShereItem *Item;
	AnsiString stTemp;
	RootItem = (TShereNameListItem*)RootNode->Data;
	for (int i = 0; i < RootNode->Count; i++) {
		Item = (TShereItem*)RootNode->Item[i]->Data;
		Item->ShereNumber = RootItem->ShereNumber;
		Item->DownPath = Dir + Item->FileName;
		Item->url = "http://" + ED_Address->Text + ":" + ED_Port->Text + "/download?number=" + Item->ShereNumber + "&id=" +
			Item->MD5;
		AddDownloadItem(Item);

		stTemp = RootItem->Name + "(" + RootItem->ShereNumber + ") -> " + Item->DownPath + " (" + Item->MD5 + ")";
		Memo1->Lines->Add(stTemp);
	}
}

void __fastcall TMainForm::ShereItemDownClick(TObject *Sender) {
	AnsiString Dir = BrowseFolder(this->Handle, "다운로드 폴더를 선택해 주세요!!!", "");
	if (Dir == "")
		return;
	Set_Form->Dir_ED->Text = Dir;
	TTreeNode *Node;
	TShereNameListItem *RootItem;
	TShereItem *Item;
	AnsiString stTemp;
	// for (int i = 0; i < TV_ShereList->SelectionCount; i++) {
	for (int i = TV_ShereList->SelectionCount - 1; i >= 0; --i) {
		Node = TV_ShereList->Selections[i];
		if (Node->Parent->Level != 0)
			break;
		RootItem = (TShereNameListItem*)Node->Parent->Data;
		if (Node->Level == 1) {
			Item = (TShereItem*)Node->Data;
			Item->ShereNumber = RootItem->ShereNumber;
			Item->DownPath = Dir + Item->FileName;
			Item->url = "http://" + ED_Address->Text + ":" + ED_Port->Text + "/download?number=" + Item->ShereNumber + "&id=" +
				Item->MD5;
			stTemp = RootItem->Name + "(" + RootItem->ShereNumber + ") -> " + Item->DownPath + " (" + Item->MD5 + ")";
			AddDownloadItem(Item);
			Memo1->Lines->Add(stTemp);
		}
	}
}

void __fastcall TMainForm::ItemDownloadClick(TObject *Sender) {
	//
	// TListItem *FItem;
	AnsiString stTemp;
	String Temp;
	TShereItem *Item;
	TListItem *fItem;
	stTemp = Item->url  + Item->DownPath;
	for (int i = 0; i < LV_DownList->Items->Count; i++) {
	   //	Memo1->Lines->Add(stTemp);
	   ShowMessage(stTemp);
	  //	if (LV_DownList->Items->Item[i]->Checked) {
//			Temp = LV_DownList->Items->Item[i]Data->url + LV_DownList->Items->Item[i]->Data->url;
		//	ShowMessage(stTemp);
		//	Memo1->Lines->Add(stTemp);
	}
}
// FileDW->Download(Item->url, Item->DownPath);


void __fastcall TMainForm::AddDownloadItem(TShereItem *DownItem) {

AnsiString Ext = ExtractFileExt(DownItem->FileName);

try {
	int idx = this->SysIconList->IsInExtSystemIcon(Ext);
	if (idx == 0)
		idx = this->SysIconList->AddExtSystemIcon(Ext);
	DownItem->IconIdx = idx;
}
catch (...) {
}

TListItem *Item = LV_DownList->Items->Add();
DownItem->ListItemIndex = Item->Index;
Item->ImageIndex = DownItem->IconIdx;
Item->Caption = " " + DownItem->FileName;
Item->SubItems->Add("0 Byte / " + MakeFileSizeString(DownItem->FileSize) + " (0%)");
Item->SubItems->Add("0 B / Sec");
Item->SubItems->Add(DownStatStr[DownItem->DownStat]);
DownItem->ListItemIndex = Item->Index;
Item->Data = DownItem;

}

void __fastcall TMainForm::LV_DownListCustomDrawItem(TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
	bool &DefaultDraw) {
if ((Item->Index % 2) == 0)
	LV_DownList->Canvas->Brush->Color = clWindow;
else
	LV_DownList->Canvas->Brush->Color = clMenuBar;
}

void __fastcall TMainForm::LV_DownListClick(TObject *Sender) {
TListView *ListView = (TListView*)Sender;
if (ListView->ItemIndex == -1)
	return;
TShereItem *Item = (TShereItem*)ListView->Items->Item[ListView->ItemIndex]->Data;
// Edit1->Text = Item->url;
}

void __fastcall TMainForm::LV_DownListSelectItem(TObject *Sender, TListItem *Item, bool Selected) {
TShereItem *dnItem = (TShereItem*)Item->Data;
// Edit1->Text = dnItem->url;
Edit1->Text = dnItem->url + dnItem->DownPath + dnItem->FileName;

}

void __fastcall TMainForm::LV_DownList_DelClick(TObject *Sender) {
if (LV_DownList->Selected == NULL)
	return;
LV_DownList->DeleteSelected();
}

void __fastcall TMainForm::SpeedButton3Click(TObject *Sender) {
// // LV_DownList->Items->Item[LV_DownList->Selected->Index + 1]->Selected = true;
// // SendMessage(LV_DownList->Handle, LVM_SETITEMPOSITION, LV_DownList->Selected->Index, false);
//
// int Index;
// TListItem *SelectedItem;
//
// if (LV_DownList->SelCount > 0) {
// Index = LV_DownList->Selected->Index;
// SelectedItem = LV_DownList->Selected;
// if (Index < LV_DownList->Items->Count - 1) {
// LV_DownList->Items->BeginUpdate();
// try {
// LV_DownList->Items->Insert(Index + 2)->Assign(SelectedItem);
// LV_DownList->Items->Delete(Index);
// LV_DownList->Items->Item[LV_DownList->Selected->Index + 1]->Selected = true;
// //		LV_DownList->Selected = LV_DownList->Items[Index + 1];
// }
// catch (...) {
// LV_DownList->Items->EndUpdate();
// SendMessage(LV_DownList->Handle, LVM_SETITEMPOSITION, LV_DownList->Selected->Index, false);
//
// }
// }
// }
}

void __fastcall TMainForm::LV_DownListDrawItem(TCustomListView *Sender, TListItem *Item, TRect &Rect, TOwnerDrawState State) {
// int Percent = 100; // 0-100
//
// LV_DownList->Canvas->Brush->Color = LV_DownList->Color;
// LV_DownList->Canvas->Font->Color = LV_DownList->Font->Color;
// LV_DownList->Canvas->FillRect(Rect);
//
// Rect.Right = (Rect.Left + LV_DownList->Columns->Items[1]->Width);
// DrawText(LV_DownList->Canvas->Handle, Item->Caption.c_str(), -1, &Rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
//
// Rect.Left += LV_DownList->Columns->Items[0]->Width;
// Rect.Right = (Rect.Left + LV_DownList->Columns->Items[2]->Width);
//
// TRect ProgressRect = Rect;
// ProgressRect.Right -= 40;
//
// ProgressRect.Left += 2;
// ProgressRect.Top += 2;
// ProgressRect.Right -= 2;
// ProgressRect.Bottom -= 2;
//
// LV_DownList->Canvas->Brush->Color = LV_DownList->Font->Color;
// LV_DownList->Canvas->FrameRect(ProgressRect);
//
// ProgressRect.Left += 1;
// ProgressRect.Top += 1;
// ProgressRect.Right -= 1;
// ProgressRect.Bottom -= 1;
//
// ProgressRect.Right = (ProgressRect.Left + ((ProgressRect.Width() * Percent) / 100));
//
// LV_DownList->Canvas->Brush->Color = clBlue;
// LV_DownList->Canvas->FillRect(ProgressRect);
//
// LV_DownList->Canvas->Brush->Color = LV_DownList->Color;
// LV_DownList->Canvas->Font->Color = LV_DownList->Font->Color;
//
// ProgressRect = Rect;
// ProgressRect.Left += 100;
//
// AnsiString str = (IntToStr(Percent) + "%");
// DrawTextA(LV_DownList->Canvas->Handle, str.c_str(), -1, &ProgressRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

}

void __fastcall TMainForm::SpeedButton10Click(TObject *Sender) {
Set_Form->ShowModal();
}
