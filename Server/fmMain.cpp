#include <vcl.h>
#include <vector>
#pragma hdrstop

#include "fmMain.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


/*
*	�����丵 : 2024.02
*
*/
TMainForm *MainForm;

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	// ������: �� ���� ���� �ʱ�ȭ
	IS_Hashed = true; // ���� �ؽ� ���� �ʱ�ȭ
	IS_Shering = false; // ���� ���� ���� �ʱ�ȭ
	this->Width = 800; // ���� �ʺ� �ʱ�ȭ
}

/*
* ��� �Լ�: FormCreate
* �� �Լ��� ���� ������ �� ȣ��Ǹ�, �ʱ� ������ �����մϴ�.
* @param Sender: �̺�Ʈ�� �߻���Ų ��ü
*/
void __fastcall TMainForm::FormCreate(TObject *Sender) {

	try {
		// ���� ������ ���� INI ������ ���� �õ�
		Shere_ini = new TIniFile(ExtractFilePath(ParamStr(0)) + "ShereInfo.ini");
	}
	catch (...) {
		// INI ���� ���� �� ���� ó��
		return;
	}

	/*
	* ��� ����: Container
	* �� ������ ���� �����̳ʸ� �����մϴ�.
	* �پ��� ��ü�� �ν��Ͻ� ����
	*/
	Container = new TContainer; // ���� �����̳� �ʱ�ȭ
	/*
	* ��� ����: SysIconList
	* �� ������ �ý��� ������ ����Ʈ�� �����մϴ�.
	*/
	SysIconList = new TExtIconList; // �ý��� ������ ����Ʈ �ʱ�ȭ
	TV_ShereList->Images = SysIconList->IconList; // TreeView�� ������ ����Ʈ ����

	// ������ �ʱ�ȭ
	initializeIcons(); // ����� ���� ������ �ʱ�ȭ �Լ� ȣ��
	Load_INI(); // INI ���Ͽ��� ���� �ε� �Լ� ȣ��

	/*
	* ��� ����: LB_WorkName, LB_Percent, LB_NowFileName
	* �� �������� ���� �۾� �̸�, �ۼ�Ʈ, ���� ���� �̸��� ǥ���ϴ� ���̺��Դϴ�.
	*/
	LB_WorkName->Caption = "";
	LB_Percent->Caption = "";
	LB_NowFileName->Caption = "";

	updateShareBtcp(); // ���� ��ư ĸ�� ������Ʈ �Լ� ȣ��

	// UI ��� �ʱ�ȭ
	WorkForWait(false); // ��� ���� �� �۾��� �����ϴ� �Լ� ȣ��
	PN_WorkProce->Visible = false;
	Btn_ShereClick(this); // ���� ��ư Ŭ�� �Լ� ȣ��
}

/*
* ��� �Լ�: initializeIcons
* �� �Լ��� ������ ���ҽ��� �ʱ�ȭ�ϰ�, �ý��� ������ ����Ʈ�� �߰��մϴ�.
*/
void __fastcall TMainForm::initializeIcons() {
    /*
    * ���� ����: icon
    * �� ������ �������� �ӽ÷� �����ϴ� �뵵�� ���˴ϴ�.
    */
	TIcon *icon = new TIcon;
    // ù ��° �������� �����ɴϴ�.
    IconRes->GetIcon(0, icon);
    // �ý��� ������ ����Ʈ�� "__Directory"��� �̸����� ù ��° �������� �߰��մϴ�.
	SysIconList->AddCustomSystemIcon("__Directory", icon);
    // ������ ��ü�� �����մϴ�.
    delete icon;

    // �� ������ ��ü�� �����մϴ�.
	icon = new TIcon;
    // �� ��° �������� �����ɴϴ�.
    IconRes->GetIcon(1, icon);
    // �ý��� ������ ����Ʈ�� "__Directory_Open"�̶�� �̸����� �� ��° �������� �߰��մϴ�.
	SysIconList->AddCustomSystemIcon("__Directory_Open", icon);
    // ������ ��ü�� �����մϴ�.
    delete icon;
}


/*
* ��� �Լ�: updateShareBtcp
* �� �Լ��� ���� ��ư�� ĸ���� ������Ʈ�մϴ�. ������ ���� ���̸� "���� ����", �׷��� ������ "���� ����"���� �����մϴ�.
*/
void __fastcall TMainForm::updateShareBtcp() {
	if (!IS_Shering)
		Btn_Shere->Caption = "���� ����";
    else
		Btn_Shere->Caption = "���� ����";
}

/*
* �̺�Ʈ �ڵ鷯: FormDestroy
* �� �Լ��� ���� �ı��� �� ȣ��Ǹ�, �Ҵ�� �޸𸮸� �����մϴ�.
* @param Sender: �� �̺�Ʈ�� Ʈ������ ��ü�Դϴ�.
*/
void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	// �Ҹ���: �Ҵ�� �޸� ����
	delete Container;   // ���� �����̳� �޸� ����
	delete Shere_ini;   // INI ���� �޸� ����
	delete SysIconList; // �ý��� ������ ����Ʈ �޸� ����
}

/*
* ��� �Լ�: Load_INI
* �� �Լ��� INI ���Ͽ��� ������ �ε��մϴ�. TreeView�� ���� �����̳ʸ� �ʱ�ȭ�ϰ�, INI ������ ������ �о TreeView�� �߰��մϴ�.
*/
void __fastcall TMainForm::Load_INI() {
	TV_ShereList->Items->Clear(); // TreeView �ʱ�ȭ
	Container->ClearShereList(); // ���� �����̳� �ʱ�ȭ

	TStringList *List = new TStringList;
	TStringList *ItemList = new TStringList;

	// INI ���Ͽ��� ���� �̸� �б�
	Shere_ini->ReadSection(INI_SHERE_LIST_NAME, List);

	for (int i = 0; i < List->Count; i++) {
		TTreeNode *rootNode = AddRootNode(TV_ShereList, List->Strings[i], false);
		ItemList->Clear();
		Shere_ini->ReadSection(List->Strings[i], ItemList);
		for (int j = 0; j < ItemList->Count; j++) {
			handleItem(List, ItemList, rootNode, i, j);
        }
	}
    // TStringList ��ü�� �����մϴ�.
	delete ItemList;
	delete List;
}

/*
* ��� �Լ�: handleItem
* �� �Լ��� INI ���Ͽ��� ���� ������ ������ ó���մϴ�. ������ ������ �����ϰ�, ���� ũ�⸦ ��Ȯ�� ���� �� ������ �������� �����̳ʿ� �߰��ϰ�, TreeView�� ��带 �߰��մϴ�.
* �׷��� ������ �ش� �����۰� ���õ� Ű�� INI ���Ͽ��� �����մϴ�.
* @param List, ItemList: INI ���Ͽ��� ���� ���� �̸��� ������ �̸��� �����ϴ� TStringList ��ü�Դϴ�.
* @param rootNode: ���� ó�� ���� ��Ʈ ����Դϴ�.
* @param i, j: ���� ó�� ���� �������� �ε����Դϴ�.
*/
void __fastcall TMainForm::handleItem(TStringList *List, TStringList *ItemList, TTreeNode *rootNode, int i, int j) {
	if (FileExists(ItemList->Strings[j])) {
		__int64 FileSize = 0;
		if (TryStrToInt64(Shere_ini->ReadString(List->Strings[i], ItemList->Strings[j], "0"), FileSize)) {
			AnsiString MD5 = Shere_ini->ReadString(INI_FILE_HASH_DONE_NAME, ItemList->Strings[j], "");
			if (MD5 == "")
				IS_Hashed = false;
			TShereItem *Item = new TShereItem(List->Strings[i], ItemList->Strings[j], MD5, FileSize);
			Container->AddItem(List->Strings[i], Item);
			AddSubNode(TV_ShereList, rootNode, ItemList->Strings[j], Item, 0, false);
		}
		else {
			deleteFileKeys(List, ItemList, i, j);
		}
    }
    else {
		deleteFileKeys(List, ItemList, i, j);
	}
}

/*
* ��� �Լ�: deleteFileKeys
* �� �Լ��� �־��� �����۰� ���õ� Ű�� INI ���Ͽ��� �����մϴ�.
* @param list, itemList: INI ���Ͽ��� ���� ���� �̸��� ������ �̸��� �����ϴ� TStringList ��ü�Դϴ�.
* @param i, j: ���� ó�� ���� �������� �ε����Դϴ�.
*/
void __fastcall TMainForm::deleteFileKeys(TStringList *list, TStringList *itemList, int i, int j) {
	Shere_ini->DeleteKey(list->Strings[i], itemList->Strings[j]);
	Shere_ini->DeleteKey(INI_FILE_HASH_DONE_NAME, itemList->Strings[j]);
}

/*
* ��� �Լ�: WorkForWait
* �� �Լ��� ��� ���¸� ǥ���ϰų� ����µ� ���˴ϴ�. ��� �г��� ǥ�� ���θ� �����ϰ�, �̹����� �ִϸ��̼��� �����մϴ�.
* @param wait: ��� ���¸� ǥ���Ϸ��� true, ������� false�� �����մϴ�.
*/
void __fastcall TMainForm::WorkForWait(bool wait) {
	Panel3->Visible = wait; // ��� �г� ǥ�� ���� ����
	try {
		((TGIFImage*)Image1->Picture->Graphic)->Animate = wait; // �ִϸ��̼� ���� ����
	}
	catch (...) {
	}
}

/*
* ��� �Լ�: AddRootNode
* �� �Լ��� �־��� �̸����� TreeView�� ��Ʈ ��带 �߰��մϴ�. ����, WriteINI�� true�� ���, INI ���Ͽ��� ��� ������ ����մϴ�.
* @param GroupList: ��带 �߰��� TreeView ��ü�Դϴ�.
* @param Name: �߰��� ����� �̸��Դϴ�.
* @param WriteINI: INI ���Ͽ� ��� ������ ������� ���θ� �����մϴ�.
* @return: �߰��� ��带 ��ȯ�մϴ�.
*/
TTreeNode * __fastcall TMainForm::AddRootNode(TTreeView *GroupList, AnsiString Name, bool WriteINI) {
	TTreeNode *Item = GroupList->Items->Add(NULL, Name);
	Item->ImageIndex = this->SysIconList->IsInExtSystemIcon("__Directory");
	Item->SelectedIndex = this->SysIconList->IsInExtSystemIcon("__Directory_Open");
	if (WriteINI)
		Shere_ini->WriteBool(INI_SHERE_LIST_NAME, Name, true);
	return Item;
}

/*
* ��� �Լ�: AddSubNode
* �� �Լ��� �־��� �̸��� ���������� TreeView�� ��Ʈ ��� �Ʒ��� ���� ��带 �߰��մϴ�. ����� �̹��� �ε����� ���� Ȯ���ڿ� ���� �����Ǹ�,
* �ش� Ȯ������ �������� �ý��� ������ ����Ʈ�� ���� ���, ���ο� �������� �߰��մϴ�. ����, WriteINI�� true�� ���, INI ���Ͽ��� ��� ������ ����մϴ�.
* @param GroupList: ��带 �߰��� TreeView ��ü�Դϴ�.
* @param RootNode: ���� ��带 �߰��� ��Ʈ ����Դϴ�.
* @param FileName: �߰��� ����� �̸�(���� �̸�)�Դϴ�.
* @param Item: �߰��� ���� ������ ��ü�Դϴ�.
* @param FileSize: �߰��� ����� ���� ũ���Դϴ�.
* @param WriteINI: INI ���Ͽ� ��� ������ ������� ���θ� �����մϴ�.
*/
void __fastcall TMainForm::AddSubNode(TTreeView *GroupList, TTreeNode *RootNode, AnsiString FileName, TObject *Item,
	__int64 FileSize, bool WriteINI) {
	// ��Ʈ ��尡 null�� �ƴ� ��쿡�� ���� ��带 �߰��մϴ�.
	if (RootNode != NULL) {
		// ��Ʈ ��� �Ʒ��� ���ο� ���� ��带 �߰��ϰ�, �� ����� ������ �����մϴ�.
		TTreeNode *SubNode = GroupList->Items->AddChildObject(RootNode, FileName, Item);
		AnsiString Ext = ExtractFileExt(FileName);
		int i = this->SysIconList->IsInExtSystemIcon(Ext);
		// Ȯ���ڿ� �ش��ϴ� �̹��� �ε����� �̹� ������ �װ��� ����ϰ�, ������ ���ο� �������� �߰��մϴ�.
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
    // WriteINI�� true�� ���, INI ���Ͽ� ��� ������ ����մϴ�.
	if (WriteINI)
		Shere_ini->WriteString(RootNode->Text, FileName, IntToStr(FileSize));
}

/*
* ��� �Լ�: Btn_MakeDirClick
* �� �Լ��� ���͸� ���� ��ư�� Ŭ���Ǿ��� �� ȣ��˴ϴ�. ���ο� ���͸� �̸��� �Է¹޾� TreeView�� ��Ʈ ���� �߰��մϴ�.
* �̹� ������ �̸��� ��尡 �����ϴ� ���, ��� �޽����� ǥ���մϴ�.
* @param Sender: ��ư Ŭ�� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::Btn_MakeDirClick(TObject *Sender) {
	MakeDirForm->IS_OK = false;
	MakeDirForm->Edit1->Text = "";
	MakeDirForm->ShowModal();
	AnsiString stTemp = MakeDirForm->Edit1->Text.Trim();
	// ��ȿ�� ���͸� �̸��� �Է¹��� ���, ���ο� ��Ʈ ��带 �߰��մϴ�.
	if (MakeDirForm->IS_OK && stTemp.Length() > 0) {
		// �̹� ������ �̸��� ��尡 �ִ��� Ȯ���մϴ�.
		for (int i = 0; i < TV_ShereList->Items->Count; i++) {
			TTreeNode *Node = (TTreeNode*)TV_ShereList->Items->Item[i];
			if (Node->Text == stTemp) {
				ShowMessage("�̹� �ִ� ���� �̸��Դϴ�.");
				return;
			}
		}
		// ���ο� ��Ʈ ��带 �߰��մϴ�.
		AddRootNode(TV_ShereList, MakeDirForm->Edit1->Text, true);
	}
}

/*
* ��� �Լ�: IsInItem
* �� �Լ��� �־��� ��忡 Ư�� �̸��� �׸��� �ִ��� Ȯ���մϴ�.
* @param Node: �׸��� Ȯ���� ����Դϴ�.
* @param Name: Ȯ���� �׸��� �̸��Դϴ�.
* @return: �׸��� ������ true, ������ false�� ��ȯ�մϴ�.
*/
bool __fastcall TMainForm::IsInItem(TTreeNode *Node, AnsiString Name) {
	for (int i = 0; i < Node->Count; i++) {
		// ��忡 �ش� �̸��� �׸��� �ִ��� Ȯ���մϴ�.
		if (Node->Item[i]->Text == Name)
			return true;
	}
	return false;
}

/*
* ��� �Լ�: TV_ShereListMouseDown
* �� �Լ��� TreeView���� ���콺 ��ư�� ������ �� ȣ��˴ϴ�. ���콺�� ���� ��ġ���� ��带 �����ϰ�, ������ ��ư�� ���ȴٸ� �ش� ��忡 ���� ���ؽ�Ʈ �޴��� ó���մϴ�.
* @param Sender: ���콺 �ٿ� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
* @param Button: ���� ���콺 ��ư�Դϴ�.
* @param Shift: Shift Ű �����Դϴ�.
* @param X: ���콺�� ���� X ��ǥ�Դϴ�.
* @param Y: ���콺�� ���� Y ��ǥ�Դϴ�.
*/
void __fastcall TMainForm::TV_ShereListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TTreeView *treeView = (TTreeView*)Sender;
	TTreeNode *node = treeView->GetNodeAt(X, Y);
	// ���콺�� ���� ��ġ�� ��尡 ���� ���, �Լ��� �����մϴ�.
	if (node == NULL)
		return;
	node->Selected = true;
	// ���콺 ������ ��ư�� ���� ��쿡�� ���ؽ�Ʈ �޴��� ó���մϴ�.
	if (Button != mbRight)
		return;

	// ���õ� ��忡 ���� ���ؽ�Ʈ �޴��� ó���մϴ�.
	HandleNodeSelection(treeView, node);
}

/*
* ��� �Լ�: HandleNodeSelection
* �� �Լ��� ���õ� ��忡 ���� ���ؽ�Ʈ �޴��� ó���մϴ�.
* @param treeView: ��� ������ ó���� TreeView ��ü�Դϴ�.
* @param node: ���õ� ����Դϴ�.
*/
void __fastcall TMainForm::HandleNodeSelection(TTreeView *treeView, TTreeNode *node) {
    // ��Ʈ ��尡 ���õ� ���
    if (node->Level == 0) {
		N1->Caption = node->Text + " ������ ���� �߰�....";
		this->PopupMenu1->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}
	// ���� ��尡 ���õ� ���
	else if (treeView->SelectionCount >= 1) {
		int selectedCount = GetCountOfSelectedItems(treeView);
		N5->Caption = IntToStr(selectedCount) + " �� �׸� ����....";
		this->PopupMenu2->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}
}

/*
* ��� �Լ�: GetCountOfSelectedItems
* �� �Լ��� TreeView���� ���õ� �׸��� ���� ��ȯ�մϴ�.
* @param treeView: �׸��� Ȯ���� TreeView ��ü�Դϴ�.
* @return: ���õ� �׸��� ���� ��ȯ�մϴ�.
*/
int __fastcall TMainForm::GetCountOfSelectedItems(TTreeView *treeView) {
	int selectedCount = 0;
	// ������ ���õ� �׸� ���� ������ 1�� ��� ī��Ʈ�� ������ŵ�ϴ�.
	for (unsigned int i = 0; i < treeView->SelectionCount; i++) {
		if (treeView->Selections[i]->Level == 1) {
			selectedCount++;
		}
	}
	return selectedCount;
}

/*
* ��� �Լ�: TV_ShereListChange
* �� �Լ��� TreeView���� ���õ� �׸��� ����Ǿ��� �� ȣ��˴ϴ�.
* ���õ� ����� ������ ���� "����" ��ư�� Ȱ��ȭ ���¿� TreeView�� ��Ƽ ���� ����� �����մϴ�.
* @param Sender: ���� ���� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
* @param Node: ���õ� ����Դϴ�.
*/
void __fastcall TMainForm::TV_ShereListChange(TObject *Sender, TTreeNode *Node) {
	if (Node->Level == 0) { // ���õ� ����� ������ 0�� ���
		Btn_DeleteDir->Enabled = true; // "����" ��ư�� Ȱ��ȭ�մϴ�.
		TV_ShereList->MultiSelect = false; // TreeView�� ��Ƽ ���� ����� ��Ȱ��ȭ�մϴ�.
	}
	else { // ���õ� ����� ������ 0�� �ƴ� ���
		Btn_DeleteDir->Enabled = false; // "����" ��ư�� ��Ȱ��ȭ�մϴ�.
		TV_ShereList->MultiSelect = true; // TreeView�� ��Ƽ ���� ����� Ȱ��ȭ�մϴ�.
	}
}


/*
* ��� �Լ�: TV_ShereListEditing
* �� �Լ��� TreeView���� ���� �̺�Ʈ�� �߻����� �� ȣ��˴ϴ�. �� �Լ������� ������ �̷������ �ʵ��� �����մϴ�.
* @param Sender: ���� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
* @param Node: �����Ƿ��� ����Դϴ�.
* @param AllowEdit: ������ ������� ���θ� �����ϴ� ���۷����Դϴ�.
*/
void __fastcall TMainForm::TV_ShereListEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit) {
	AllowEdit = false;
}

/*
* ��� �Լ�: Btn_DeleteDirClick
* �� �Լ��� "����" ��ư�� Ŭ���Ǿ��� �� ȣ��˴ϴ�. ���õ� ���� �� ���� ������ �����մϴ�.
* @param Sender: ��ư Ŭ�� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::Btn_DeleteDirClick(TObject *Sender) {
	// ���õ� ��尡 ���� ��� �Լ��� �����մϴ�.
	if (TV_ShereList->Selected == NULL) return;

	if (TV_ShereList->Selected->Level == 0) { // ���õ� ����� ������ 0�� ���
		AnsiString confirmationMsg = TV_ShereList->Selected->Text + " ������ ���� �Ͻðڽ��ϱ�?";
		// ����ڰ� ������ Ȯ���ϸ� ���õ� ���� �� ���� ������ �����ϰ�, ���� �����͵� �����մϴ�.
		if (MessageBoxA(this->Handle, confirmationMsg.c_str(), "Ȯ��", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			TTreeNode *RootNode = TV_ShereList->Selected; // ���õ� ��带 RootNode�� �����մϴ�.
			DeleteSubNodes(RootNode); // RootNode�� �� ���� ������ �����մϴ�.

			// ���� �����͸� �����մϴ�.
			Container->DeleteShereListFromName(RootNode->Text);
			Shere_ini->DeleteKey(INI_SHERE_LIST_NAME, RootNode->Text);
			Shere_ini->EraseSection(RootNode->Text);
			TV_ShereList->Selected->Delete(); // TreeView���� ���õ� ��带 �����մϴ�.
		}
	}
}


/*
* ��� �Լ�: DeleteSubNodes
* �� �Լ��� �־��� ��Ʈ ����� ���� ��带 ��� �����մϴ�.
* @param RootNode: ���� ��带 ������ ��Ʈ ����Դϴ�.
*/
void __fastcall TMainForm::DeleteSubNodes(TTreeNode *RootNode) {
	for (int i = RootNode->Count - 1; i >= 0; --i) {
		TTreeNode *Node = RootNode->Item[i];
		// ������ 1�� ��带 �����մϴ�.
		if (Node->Level == 1) {
			TShereItem *Item = (TShereItem*)Node->Data;
			Container->DeleteItem(Item->Name, Item);
			Shere_ini->DeleteKey(Node->Parent->Text, Node->Text);
			Node->Delete();
		}
	}
}

/*
* ��� �Լ�: N1Click
* �� �Լ��� "������ �߰�" ���ؽ�Ʈ �޴��� Ŭ���Ǿ��� �� ȣ��˴ϴ�. ���õ� ��忡 ������ �߰��մϴ�.
* @param Sender: �޴� Ŭ�� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::N1Click(TObject *Sender) {
	TTreeNode *Node = TV_ShereList->Selected;
	AddShereFile(Node);
}

/*
* ��� �Լ�: AddShereFile
* �� �Լ��� ���õ� ��忡 ������ �߰��մϴ�.
* @param Node: ������ �߰��� ����Դϴ�.
*/
void __fastcall TMainForm::AddShereFile(TTreeNode *Node) {
	if (Node == NULL || Node->Level != 0)
		return;
	if (!OpenDialog1->Execute())
		return;
	// UI ������Ʈ �� ���� ó��
	LB_WorkName->Caption = "���� �߰� ��...";
	WorkForWait(true);
	TV_ShereList->Items->BeginUpdate();

	for (int i = 0; i < OpenDialog1->Files->Count; i++) {
		AnsiString filePath = OpenDialog1->Files->Strings[i];
		if (!IsInItem(Node, filePath)) {
			ProcessFile(filePath, Node);
		}
		Application->ProcessMessages();
	}
	Node->Expand(true);
	TV_ShereList->Items->EndUpdate();
	WorkForWait(false);
	IS_Hashed = false;
}

/*
* ��� �Լ�: ProcessFile
* �� �Լ��� �־��� ���� ����� ������ ó���մϴ�.
* @param filePath: ó���� ������ ����Դϴ�.
* @param Node: ������ �߰��� ����Դϴ�.
*/
void __fastcall TMainForm::ProcessFile(const AnsiString &filePath, TTreeNode *Node) {
	__int64 FileSize;
	if (FileExists(filePath)) {
		FileSize = GetLocalFileSize(filePath);
		if (FileSize >= MIN_SHERE_FILE_SIZE && FileSize < MAX_SHERE_FILE_SIZE) {
			TShereItem *Item = new TShereItem(Node->Text, filePath, Shere_ini->ReadString(INI_FILE_HASH_DONE_NAME, filePath, ""), FileSize);
			Container->AddItem(Item->Name, Item);
			AddSubNode(TV_ShereList, Node, filePath, Item, FileSize, true);
		}
	}
}

/*
* ��� �Լ�: DeleteShere
* �� �Լ��� TreeView���� ���õ� �׸��� �����մϴ�.
* @param Sender: ���� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::DeleteShere(TObject *Sender) {
	for (int i = TV_ShereList->SelectionCount - 1; i >= 0; --i) {
		TTreeNode *Node = TV_ShereList->Selections[i];
		if (Node->Level == 1) {
			TShereItem *Item = (TShereItem*)Node->Data;
			Container->DeleteItem(Item->Name, Item);
			Shere_ini->DeleteKey(Node->Parent->Text, Node->Text);
			Node->Delete();
		}
	}
}

/*
* ��� �Լ�: N5Click
* �� �Լ��� "���� ����" ���ؽ�Ʈ �޴��� Ŭ���Ǿ��� �� ȣ��˴ϴ�. ���õ� �׸��� �����մϴ�.
* @param Sender: �޴� Ŭ�� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::N5Click(TObject *Sender) {
	DeleteShere(Sender);
}

/*
* ��� �Լ�: TV_ShereListKeyDown
* �� �Լ��� TreeView���� Ű�� ������ �� ȣ��˴ϴ�. Delete Ű�� ������ ���õ� �׸��� �����մϴ�.
* @param Sender: Ű �ٿ� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
* @param Key: ���� Ű�� �ڵ��Դϴ�.
* @param Shift: Shift Ű �����Դϴ�.
*/
void __fastcall TMainForm::TV_ShereListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
	if (Key == VK_DELETE) {
		TTreeView *treeView = (TTreeView*)Sender;
		if (treeView->SelectionCount >= 1) {
			int selectedCount = GetCountOfSelectedItems(treeView);
			AnsiString stTemp = IntToStr(selectedCount) + " ���� �׸��� ���� �Ͻðڽ��ϱ�?";
			if (MessageBoxA(this->Handle, stTemp.c_str(), "Ȯ��", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				DeleteShere(Sender);
			}
		}
	}
}


/*
* ��� �Լ�: Btn_ShereClick
* �� �Լ��� "����" ��ư�� Ŭ���Ǿ��� �� ȣ��˴ϴ�.
* ���� ���̸� ������ �����ϰ�, �׷��� ������ ������ �����մϴ�.
* @param Sender: ��ư Ŭ�� �̺�Ʈ�� �߻���Ų ��ü�Դϴ�.
*/
void __fastcall TMainForm::Btn_ShereClick(TObject *Sender) {
	if (IS_Shering) { // ���� ���� ���� ���
		StopSharing(); // ������ �����մϴ�.
	}
    else { // ���� ���� �ƴ� ���
		StartSharing(); // ������ �����մϴ�.
    }
}

/*
* ��� �Լ�: StartSharing
* �� �Լ��� ������ �����մϴ�.
* ���� ���� ���¸� ��Ÿ���� ������ �����ϰ�, ���� UI�� ��Ȱ��ȭ�մϴ�.
* ������ �ؽ� ó���� �����ϰ�, ���� ������ Ȱ��ȭ�մϴ�.
*/
void __fastcall TMainForm::StartSharing() {
	Btn_Shere->Caption = "���� ����"; // ��ư�� ���̺��� �����մϴ�.
	IS_Shering = true; // ���� ���� ���¸� ��Ÿ���� ������ true�� �����մϴ�.

	// UI�� ��Ȱ��ȭ�մϴ�.
	Btn_MakeDir->Enabled = false;
	Btn_DeleteDir->Enabled = false;
	TV_ShereList->Enabled = false;
	Btn_Shere->Enabled = false;

	if (HashingProce()) { // ������ �ؽ� ó���� �����մϴ�. �����ϸ� true�� ��ȯ�մϴ�.
		// UI�� ������Ʈ�մϴ�.
		Btn_MakeDir->Enabled = false;
		Btn_DeleteDir->Enabled = false;
		TV_ShereList->Enabled = false;
		Btn_Shere->Enabled = true;

		Container->ClearSendingList(); // ���� ����Ʈ�� �ʱ�ȭ�մϴ�.

		try {
			HTTPS->Active = true; // ���� ������ Ȱ��ȭ�մϴ�.
		}
		catch (...) { // ������ Ȱ��ȭ�ϴ� �� ������ ���
			ShowMessage("���� ������ ������ �� �����ϴ�");
			Btn_ShereClick(NULL);
		}
	}
	else { // �ؽ� ó���� ������ ���
		Btn_ShereClick(NULL);
	}
}

/*
* ��� �Լ�: StopSharing
* �� �Լ��� ������ �����մϴ�.
* ���� ���� ���¸� ��Ÿ���� ������ �����ϰ�, ���� UI�� Ȱ��ȭ�մϴ�.
* ���� ������ ��Ȱ��ȭ�մϴ�.
*/
void __fastcall TMainForm::StopSharing() {
	Btn_Shere->Caption = "���� ����"; // ��ư�� ���̺��� �����մϴ�.
	IS_Shering = false; // ���� ���� ���¸� ��Ÿ���� ������ false�� �����մϴ�.

	// UI�� Ȱ��ȭ�մϴ�.
	Btn_Shere->Enabled = true;
	Btn_MakeDir->Enabled = true;
	Btn_DeleteDir->Enabled = true;
	TV_ShereList->Enabled = true;

	try {
		HTTPS->Active = false; // ���� ������ ��Ȱ��ȭ�մϴ�.
	}
	catch (...) { // ������ ��Ȱ��ȭ�ϴ� �� ������ ���
	}
}


/*
* ��� �Լ�: HashingProce
* �� �Լ��� ��ü ������ ũ�⸦ ����ϰ�, �� ���Ͽ� ���� MD5 �ؽø� ����ϴ� ������ �մϴ�.
* @return bool: ��� ������ �ؽ� ����� ���������� �Ϸ�Ǹ� true��, �׷��� ������ false�� ��ȯ�մϴ�.
*/
bool __fastcall TMainForm::HashingProce() {

    /*
    * ��� ����: TotalFileSize
    * �� ������ ó���� ���ϵ��� ��ü ũ�⸦ �����մϴ�.
    */
    TotalFileSize = 0;

    /*
    * ��� ����: EndPos
    * �� ������ ������� ó���� ������ ũ�⸦ �����մϴ�.
    */
    EndPos = 0;

    // ���� ����� ��ȸ�ϸ� ��ü ���� ũ�⸦ ����մϴ�.
    for (int i = 0; i < TV_ShereList->Items->Count; i++) {
        TTreeNode *RootNode = (TTreeNode*)TV_ShereList->Items->Item[i];
		for (int j = 0; j < RootNode->Count; j++) {
			TTreeNode *Node;
			if (RootNode->Item[j] != NULL)
				Node = RootNode->Item[j];
			TShereItem *p = (TShereItem*)Node->Data;
			TotalFileSize += p->FileSize;
		}
    }

    // ��ü ���� ũ�Ⱑ 0 ���ϸ� �ؽ� ����� ������ �� �����Ƿ� false�� ��ȯ�մϴ�.
    if (TotalFileSize <= 0)
        return false;

    // �ؽ� ��� ��� �÷��׸� �ʱ�ȭ�մϴ�.
    IS_HashCencel = false;

    // �۾� Ÿ�̸Ӹ� Ȱ��ȭ�ϰ�, �۾� ���¸� "���� Ȯ����"���� ǥ���մϴ�.
    WorkTimer->Enabled = true;
    LB_WorkName->Caption = "���� Ȯ����...";
    WorkForWait(true);
    PN_WorkProce->Visible = true;

    // ���� ����� ��ȸ�ϸ� �� ���Ͽ� ���� MD5 �ؽø� ����մϴ�.
    for (int i = 0; i < TV_ShereList->Items->Count; i++) {
		if (IS_HashCencel) {
			PN_WorkProce->Visible = false;
			WorkForWait(false);
			WorkTimer->Enabled = false;
			return false;
		}

		TTreeNode *RootNode = (TTreeNode*)TV_ShereList->Items->Item[i];
		for (int j = 0; j < RootNode->Count; j++) {
			TTreeNode *Node;
			if (RootNode->Item[j] != NULL)
				Node = RootNode->Item[j];
			TShereItem *p = (TShereItem*)Node->Data;
			LB_NowFileName->Caption = p->FileName;
			if (p->MD5 != "")
				EndPos += p->FileSize;
			else {
				p->MD5 = GetMD5HashProce(p->FileName);
			}
		}
	}

    // �ؽ� ����� �Ϸ�Ǿ����Ƿ� �۾� Ÿ�̸Ӹ� ��Ȱ��ȭ�ϰ�, �۾� ���¸� ����ϴ�.
    PN_WorkProce->Visible = false;
    WorkForWait(false);
    WorkTimer->Enabled = false;
    IS_HashCencel = false;

    // ��� ������ �ؽ� ����� ���������� �Ϸ�Ǿ����Ƿ� true�� ��ȯ�մϴ�.
    return true;
}

/*
* ��� �Լ�: GetMD5HashProce
* �� �Լ��� �־��� ������ MD5 �ؽð��� ����մϴ�.
* @param FileName: MD5 �ؽð��� ����� ������ �̸�
* @return AnsiString: ���� MD5 �ؽð��� ���ڿ��� ��ȯ�մϴ�. ��꿡 �����ϸ� �� ���ڿ��� ��ȯ�մϴ�.
*/
AnsiString __fastcall TMainForm::GetMD5HashProce(AnsiString FileName) {

	// DWORD dwStatus = 0;
	BOOL bResult;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

    // ������ �б� ���� �ڵ�
	HANDLE hFile = NULL;
    // ������ ������ �о ������ ����
	BYTE rgbFile[BUFSIZE];
    // ���Ͽ��� ���� ����Ʈ ��
	DWORD cbRead = 0;
    // MD5 �ؽð��� ������ ����
	BYTE rgbHash[MD5LEN];
    // MD5 �ؽð��� ����
	DWORD cbHash = 0;

    // MD5 �ؽð��� ������ ���ڿ�
	AnsiString MD5Hash = "";
     /*
    * ��� ����: hFile
    * �� ������ ������ �б� ���� �ڵ��� �����մϴ�.
	*/
    // ������ ����
	hFile = CreateFileA(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    // ���� ���� ���� ��
	if (INVALID_HANDLE_VALUE == hFile)
		return "";

    // ��ȣȭ ���� ������ �ڵ� ȹ��
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return "";

    // MD5 �ؽ� ��ü ����
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	try {
        // ������ ������ �о MD5 �ؽ� ���
		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL)) {
            // �ؽ� ��� ��� ��
			if (IS_HashCencel) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return "";
			}

            // ������ �� ���� ��
			if (cbRead == 0)
				break;
			EndPos += cbRead;
            // MD5 �ؽ� ������ �߰�
			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return "";
			}
            // UI ������Ʈ�� ���� �޽��� ó��
			Application->ProcessMessages();
		}
	}
	catch (Exception &E) {
		bResult = FALSE;
	}

    // ���� �б� ���� ��
	if (!bResult) {
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return "";
	}

	cbHash = MD5LEN;
    // MD5 �ؽð� ȹ��
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		AnsiString stTemp = "";
        // MD5 �ؽð��� ���ڿ��� ��ȯ
		for (DWORD i = 0; i < cbHash; i++) {
			MD5Hash = MD5Hash + stTemp.sprintf("%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
		}
	}
	else
		MD5Hash = "";
	MD5Hash = MD5Hash.UpperCase();
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);
	// MD5 �ؽð��� ���Ǹ� ���ϸ�� �Բ� ����
	if (MD5Hash != "")
		this->Shere_ini->WriteString(INI_FILE_HASH_DONE_NAME, FileName, MD5Hash);
	return MD5Hash;
}


void __fastcall TMainForm::TotalWorkProce() {
 // ��ü �۾��� ���� ��Ȳ�� ������� ����մϴ�.
    double progress = (double)EndPos / (double)TotalFileSize * 100;

    // ���α׷��� ���� ��ġ�� ������Ʈ�մϴ�.
    PB_Total->Position = static_cast<int>(progress);

    // ���� ��Ȳ�� �ؽ�Ʈ�� ǥ���մϴ�.
	LB_Percent->Caption = FormatFloat("00.00 %", progress);
}

void __fastcall TMainForm::FormResize(TObject *Sender) {
	if (this->Width < 800)
		this->Width = 800;
}

// HTTPS GET ��û�� ó���ϴ� �Լ�
void __fastcall TMainForm::HTTPSCommandGet(TIdContext *AContext, TIdHTTPRequestInfo *ARequestInfo,
	TIdHTTPResponseInfo *AResponseInfo) {
    // favicon.ico ��û�� ó������ ����
	if (ARequestInfo->Document == "/favicon.ico")
        return;

    // HTTP ��û ������ �α׷� ���
	LogHTTPRequest(ARequestInfo);

    // UserAgent�� ���� ��û ó�� ��� �б�
    if (ARequestInfo->UserAgent != DOWNLOADER_USER_AGENT_STRING) {
        // �Ϲ� ��û ó��
        ProcessRegularRequest(ARequestInfo, AResponseInfo);
    } else {
        // �ٿ�δ� ��û ó��
        ProcessDownloaderRequest(ARequestInfo, AResponseInfo);
    }
}

// HTTP ��û ������ �α׷� ����ϴ� �Լ�
void __fastcall TMainForm::LogHTTPRequest(TIdHTTPRequestInfo *ARequestInfo) {
    // �� ������ �α׷� ���
    Memo1->Lines->Add("UserAgent -> " + ARequestInfo->UserAgent);
    Memo1->Lines->Add("Command -> " + ARequestInfo->Command);
    Memo1->Lines->Add("Document -> " + ARequestInfo->Document);
    Memo1->Lines->Add("Params -> " + ARequestInfo->Params->Text);
    Memo1->Lines->Add("Ranges -> " + ARequestInfo->Range);
    Memo1->Lines->Add("Host -> " + ARequestInfo->Host);
}

// �Ϲ� ��û�� ó���ϴ� �Լ�
void __fastcall TMainForm::ProcessRegularRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo) {
    // ��û�� Document�� ���� ó�� ��� �б�
	if (ARequestInfo->Document == "/download") {
        // ������ ������ ����
        SendFileToWeb(AResponseInfo, Container, ARequestInfo->Params);
    } else if (ARequestInfo->Document == "/") {
        // �� ���� ������ ����
        SendWebBodyPage(AResponseInfo, Container);
    } else {
        // ��û�� ������ ������ ����
        SendRequestFileWeb(ARequestInfo->Document, AResponseInfo);
    }
}

// �ٿ�δ� ��û�� ó���ϴ� �Լ�
void __fastcall TMainForm::ProcessDownloaderRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo) {
    // HTTP ��û�� Command Ÿ�Կ� ���� ó�� ��� �б�
    if (ARequestInfo->Command == "HEAD") {
        // HEAD ��û ��, ���� ũ�� ������ HTTP ��� ������ ����
		SendFileSizeFromHTTPHeadInfo(AResponseInfo, Container, ARequestInfo->Params);
	} else if (ARequestInfo->Command == "GET") {
		// GET ��û ��, ��û�� Document�� ���� ó�� ��� �б�
		if(ARequestInfo->Document == "/shere_list") {
			// "/shere_list" ��û ��, ���� ����Ʈ ������ ����
			SendShereList(AResponseInfo, Container);
		} else if (ARequestInfo->Document == "/shere_item_list") {
			// "/shere_item_list" ��û ��, ���� ������ ����Ʈ ������ ����
            SendShereItemList(AResponseInfo, Container, ARequestInfo->Params);
		} else if (ARequestInfo->Document == "/download") {
			// "/download" ��û ��, ��û�� ������ Ŭ���̾�Ʈ�� ����
			// SendFileToClient(ARequestInfo->Range, AResponseInfo, Container, ARequestInfo->Params);
        }
	}
}


void __fastcall TMainForm::WorkTimerTimer(TObject *Sender) {
	TotalWorkProce();
}

void __fastcall TMainForm::Btn_HashCencelClick(TObject *Sender) {
	IS_HashCencel = true;
}
