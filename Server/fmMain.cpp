#include <vcl.h>
#include <vector>
#pragma hdrstop

#include "fmMain.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


/*
*	리팩토링 : 2024.02
*
*/
TMainForm *MainForm;

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	// 생성자: 몇 가지 변수 초기화
	IS_Hashed = true; // 파일 해시 여부 초기화
	IS_Shering = false; // 파일 공유 여부 초기화
	this->Width = 800; // 폼의 너비 초기화
}

/*
* 멤버 함수: FormCreate
* 이 함수는 폼이 생성될 때 호출되며, 초기 설정을 수행합니다.
* @param Sender: 이벤트를 발생시킨 객체
*/
void __fastcall TMainForm::FormCreate(TObject *Sender) {

	try {
		// 공유 정보를 위한 INI 파일을 생성 시도
		Shere_ini = new TIniFile(ExtractFilePath(ParamStr(0)) + "ShereInfo.ini");
	}
	catch (...) {
		// INI 파일 생성 중 예외 처리
		return;
	}

	/*
	* 멤버 변수: Container
	* 이 변수는 파일 컨테이너를 저장합니다.
	* 다양한 객체의 인스턴스 생성
	*/
	Container = new TContainer; // 파일 컨테이너 초기화
	/*
	* 멤버 변수: SysIconList
	* 이 변수는 시스템 아이콘 리스트를 저장합니다.
	*/
	SysIconList = new TExtIconList; // 시스템 아이콘 리스트 초기화
	TV_ShereList->Images = SysIconList->IconList; // TreeView에 아이콘 리스트 설정

	// 아이콘 초기화
	initializeIcons(); // 사용자 정의 아이콘 초기화 함수 호출
	Load_INI(); // INI 파일에서 정보 로드 함수 호출

	/*
	* 멤버 변수: LB_WorkName, LB_Percent, LB_NowFileName
	* 이 변수들은 각각 작업 이름, 퍼센트, 현재 파일 이름을 표시하는 레이블입니다.
	*/
	LB_WorkName->Caption = "";
	LB_Percent->Caption = "";
	LB_NowFileName->Caption = "";

	updateShareBtcp(); // 공유 버튼 캡션 업데이트 함수 호출

	// UI 요소 초기화
	WorkForWait(false); // 대기 중일 때 작업을 수행하는 함수 호출
	PN_WorkProce->Visible = false;
	Btn_ShereClick(this); // 공유 버튼 클릭 함수 호출
}

/*
* 멤버 함수: initializeIcons
* 이 함수는 아이콘 리소스를 초기화하고, 시스템 아이콘 리스트에 추가합니다.
*/
void __fastcall TMainForm::initializeIcons() {
    /*
    * 로컬 변수: icon
    * 이 변수는 아이콘을 임시로 저장하는 용도로 사용됩니다.
    */
	TIcon *icon = new TIcon;
    // 첫 번째 아이콘을 가져옵니다.
    IconRes->GetIcon(0, icon);
    // 시스템 아이콘 리스트에 "__Directory"라는 이름으로 첫 번째 아이콘을 추가합니다.
	SysIconList->AddCustomSystemIcon("__Directory", icon);
    // 아이콘 객체를 삭제합니다.
    delete icon;

    // 새 아이콘 객체를 생성합니다.
	icon = new TIcon;
    // 두 번째 아이콘을 가져옵니다.
    IconRes->GetIcon(1, icon);
    // 시스템 아이콘 리스트에 "__Directory_Open"이라는 이름으로 두 번째 아이콘을 추가합니다.
	SysIconList->AddCustomSystemIcon("__Directory_Open", icon);
    // 아이콘 객체를 삭제합니다.
    delete icon;
}


/*
* 멤버 함수: updateShareBtcp
* 이 함수는 공유 버튼의 캡션을 업데이트합니다. 공유가 진행 중이면 "공유 중지", 그렇지 않으면 "공유 시작"으로 설정합니다.
*/
void __fastcall TMainForm::updateShareBtcp() {
	if (!IS_Shering)
		Btn_Shere->Caption = "공유 시작";
    else
		Btn_Shere->Caption = "공유 중지";
}

/*
* 이벤트 핸들러: FormDestroy
* 이 함수는 폼이 파괴될 때 호출되며, 할당된 메모리를 해제합니다.
* @param Sender: 이 이벤트를 트리거한 객체입니다.
*/
void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	// 소멸자: 할당된 메모리 해제
	delete Container;   // 파일 컨테이너 메모리 해제
	delete Shere_ini;   // INI 파일 메모리 해제
	delete SysIconList; // 시스템 아이콘 리스트 메모리 해제
}

/*
* 멤버 함수: Load_INI
* 이 함수는 INI 파일에서 정보를 로드합니다. TreeView와 파일 컨테이너를 초기화하고, INI 파일의 내용을 읽어서 TreeView에 추가합니다.
*/
void __fastcall TMainForm::Load_INI() {
	TV_ShereList->Items->Clear(); // TreeView 초기화
	Container->ClearShereList(); // 파일 컨테이너 초기화

	TStringList *List = new TStringList;
	TStringList *ItemList = new TStringList;

	// INI 파일에서 섹션 이름 읽기
	Shere_ini->ReadSection(INI_SHERE_LIST_NAME, List);

	for (int i = 0; i < List->Count; i++) {
		TTreeNode *rootNode = AddRootNode(TV_ShereList, List->Strings[i], false);
		ItemList->Clear();
		Shere_ini->ReadSection(List->Strings[i], ItemList);
		for (int j = 0; j < ItemList->Count; j++) {
			handleItem(List, ItemList, rootNode, i, j);
        }
	}
    // TStringList 객체를 삭제합니다.
	delete ItemList;
	delete List;
}

/*
* 멤버 함수: handleItem
* 이 함수는 INI 파일에서 읽은 아이템 정보를 처리합니다. 파일이 실제로 존재하고, 파일 크기를 정확히 읽을 수 있으면 아이템을 컨테이너에 추가하고, TreeView에 노드를 추가합니다.
* 그렇지 않으면 해당 아이템과 관련된 키를 INI 파일에서 삭제합니다.
* @param List, ItemList: INI 파일에서 읽은 섹션 이름과 아이템 이름을 저장하는 TStringList 객체입니다.
* @param rootNode: 현재 처리 중인 루트 노드입니다.
* @param i, j: 현재 처리 중인 아이템의 인덱스입니다.
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
* 멤버 함수: deleteFileKeys
* 이 함수는 주어진 아이템과 관련된 키를 INI 파일에서 삭제합니다.
* @param list, itemList: INI 파일에서 읽은 섹션 이름과 아이템 이름을 저장하는 TStringList 객체입니다.
* @param i, j: 현재 처리 중인 아이템의 인덱스입니다.
*/
void __fastcall TMainForm::deleteFileKeys(TStringList *list, TStringList *itemList, int i, int j) {
	Shere_ini->DeleteKey(list->Strings[i], itemList->Strings[j]);
	Shere_ini->DeleteKey(INI_FILE_HASH_DONE_NAME, itemList->Strings[j]);
}

/*
* 멤버 함수: WorkForWait
* 이 함수는 대기 상태를 표시하거나 숨기는데 사용됩니다. 대기 패널의 표시 여부를 설정하고, 이미지의 애니메이션을 제어합니다.
* @param wait: 대기 상태를 표시하려면 true, 숨기려면 false를 설정합니다.
*/
void __fastcall TMainForm::WorkForWait(bool wait) {
	Panel3->Visible = wait; // 대기 패널 표시 여부 설정
	try {
		((TGIFImage*)Image1->Picture->Graphic)->Animate = wait; // 애니메이션 여부 설정
	}
	catch (...) {
	}
}

/*
* 멤버 함수: AddRootNode
* 이 함수는 주어진 이름으로 TreeView에 루트 노드를 추가합니다. 또한, WriteINI가 true인 경우, INI 파일에도 노드 정보를 기록합니다.
* @param GroupList: 노드를 추가할 TreeView 객체입니다.
* @param Name: 추가할 노드의 이름입니다.
* @param WriteINI: INI 파일에 노드 정보를 기록할지 여부를 결정합니다.
* @return: 추가된 노드를 반환합니다.
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
* 멤버 함수: AddSubNode
* 이 함수는 주어진 이름과 아이템으로 TreeView의 루트 노드 아래에 하위 노드를 추가합니다. 노드의 이미지 인덱스는 파일 확장자에 따라 결정되며,
* 해당 확장자의 아이콘이 시스템 아이콘 리스트에 없는 경우, 새로운 아이콘을 추가합니다. 또한, WriteINI가 true인 경우, INI 파일에도 노드 정보를 기록합니다.
* @param GroupList: 노드를 추가할 TreeView 객체입니다.
* @param RootNode: 하위 노드를 추가할 루트 노드입니다.
* @param FileName: 추가할 노드의 이름(파일 이름)입니다.
* @param Item: 추가할 노드와 연관된 객체입니다.
* @param FileSize: 추가할 노드의 파일 크기입니다.
* @param WriteINI: INI 파일에 노드 정보를 기록할지 여부를 결정합니다.
*/
void __fastcall TMainForm::AddSubNode(TTreeView *GroupList, TTreeNode *RootNode, AnsiString FileName, TObject *Item,
	__int64 FileSize, bool WriteINI) {
	// 루트 노드가 null이 아닌 경우에만 하위 노드를 추가합니다.
	if (RootNode != NULL) {
		// 루트 노드 아래에 새로운 하위 노드를 추가하고, 그 노드의 정보를 설정합니다.
		TTreeNode *SubNode = GroupList->Items->AddChildObject(RootNode, FileName, Item);
		AnsiString Ext = ExtractFileExt(FileName);
		int i = this->SysIconList->IsInExtSystemIcon(Ext);
		// 확장자에 해당하는 이미지 인덱스가 이미 있으면 그것을 사용하고, 없으면 새로운 아이콘을 추가합니다.
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
    // WriteINI가 true인 경우, INI 파일에 노드 정보를 기록합니다.
	if (WriteINI)
		Shere_ini->WriteString(RootNode->Text, FileName, IntToStr(FileSize));
}

/*
* 멤버 함수: Btn_MakeDirClick
* 이 함수는 디렉터리 생성 버튼이 클릭되었을 때 호출됩니다. 새로운 디렉터리 이름을 입력받아 TreeView에 루트 노드로 추가합니다.
* 이미 동일한 이름의 노드가 존재하는 경우, 경고 메시지를 표시합니다.
* @param Sender: 버튼 클릭 이벤트를 발생시킨 객체입니다.
*/
void __fastcall TMainForm::Btn_MakeDirClick(TObject *Sender) {
	MakeDirForm->IS_OK = false;
	MakeDirForm->Edit1->Text = "";
	MakeDirForm->ShowModal();
	AnsiString stTemp = MakeDirForm->Edit1->Text.Trim();
	// 유효한 디렉터리 이름을 입력받은 경우, 새로운 루트 노드를 추가합니다.
	if (MakeDirForm->IS_OK && stTemp.Length() > 0) {
		// 이미 동일한 이름의 노드가 있는지 확인합니다.
		for (int i = 0; i < TV_ShereList->Items->Count; i++) {
			TTreeNode *Node = (TTreeNode*)TV_ShereList->Items->Item[i];
			if (Node->Text == stTemp) {
				ShowMessage("이미 있는 공유 이름입니다.");
				return;
			}
		}
		// 새로운 루트 노드를 추가합니다.
		AddRootNode(TV_ShereList, MakeDirForm->Edit1->Text, true);
	}
}

/*
* 멤버 함수: IsInItem
* 이 함수는 주어진 노드에 특정 이름의 항목이 있는지 확인합니다.
* @param Node: 항목을 확인할 노드입니다.
* @param Name: 확인할 항목의 이름입니다.
* @return: 항목이 있으면 true, 없으면 false를 반환합니다.
*/
bool __fastcall TMainForm::IsInItem(TTreeNode *Node, AnsiString Name) {
	for (int i = 0; i < Node->Count; i++) {
		// 노드에 해당 이름의 항목이 있는지 확인합니다.
		if (Node->Item[i]->Text == Name)
			return true;
	}
	return false;
}

/*
* 멤버 함수: TV_ShereListMouseDown
* 이 함수는 TreeView에서 마우스 버튼이 눌렸을 때 호출됩니다. 마우스가 눌린 위치에서 노드를 선택하고, 오른쪽 버튼이 눌렸다면 해당 노드에 대한 컨텍스트 메뉴를 처리합니다.
* @param Sender: 마우스 다운 이벤트를 발생시킨 객체입니다.
* @param Button: 눌린 마우스 버튼입니다.
* @param Shift: Shift 키 상태입니다.
* @param X: 마우스가 눌린 X 좌표입니다.
* @param Y: 마우스가 눌린 Y 좌표입니다.
*/
void __fastcall TMainForm::TV_ShereListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TTreeView *treeView = (TTreeView*)Sender;
	TTreeNode *node = treeView->GetNodeAt(X, Y);
	// 마우스가 눌린 위치에 노드가 없는 경우, 함수를 종료합니다.
	if (node == NULL)
		return;
	node->Selected = true;
	// 마우스 오른쪽 버튼이 눌린 경우에만 컨텍스트 메뉴를 처리합니다.
	if (Button != mbRight)
		return;

	// 선택된 노드에 따라 컨텍스트 메뉴를 처리합니다.
	HandleNodeSelection(treeView, node);
}

/*
* 멤버 함수: HandleNodeSelection
* 이 함수는 선택된 노드에 따라 컨텍스트 메뉴를 처리합니다.
* @param treeView: 노드 선택을 처리할 TreeView 객체입니다.
* @param node: 선택된 노드입니다.
*/
void __fastcall TMainForm::HandleNodeSelection(TTreeView *treeView, TTreeNode *node) {
    // 루트 노드가 선택된 경우
    if (node->Level == 0) {
		N1->Caption = node->Text + " 공유에 파일 추가....";
		this->PopupMenu1->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}
	// 하위 노드가 선택된 경우
	else if (treeView->SelectionCount >= 1) {
		int selectedCount = GetCountOfSelectedItems(treeView);
		N5->Caption = IntToStr(selectedCount) + " 개 항목 삭제....";
		this->PopupMenu2->Popup(((TPoint)Mouse->CursorPos).X, ((TPoint)Mouse->CursorPos).Y);
	}
}

/*
* 멤버 함수: GetCountOfSelectedItems
* 이 함수는 TreeView에서 선택된 항목의 수를 반환합니다.
* @param treeView: 항목을 확인할 TreeView 객체입니다.
* @return: 선택된 항목의 수를 반환합니다.
*/
int __fastcall TMainForm::GetCountOfSelectedItems(TTreeView *treeView) {
	int selectedCount = 0;
	// 각각의 선택된 항목에 대해 레벨이 1인 경우 카운트를 증가시킵니다.
	for (unsigned int i = 0; i < treeView->SelectionCount; i++) {
		if (treeView->Selections[i]->Level == 1) {
			selectedCount++;
		}
	}
	return selectedCount;
}

/*
* 멤버 함수: TV_ShereListChange
* 이 함수는 TreeView에서 선택된 항목이 변경되었을 때 호출됩니다.
* 선택된 노드의 레벨에 따라 "삭제" 버튼의 활성화 상태와 TreeView의 멀티 선택 기능을 설정합니다.
* @param Sender: 선택 변경 이벤트를 발생시킨 객체입니다.
* @param Node: 선택된 노드입니다.
*/
void __fastcall TMainForm::TV_ShereListChange(TObject *Sender, TTreeNode *Node) {
	if (Node->Level == 0) { // 선택된 노드의 레벨이 0인 경우
		Btn_DeleteDir->Enabled = true; // "삭제" 버튼을 활성화합니다.
		TV_ShereList->MultiSelect = false; // TreeView의 멀티 선택 기능을 비활성화합니다.
	}
	else { // 선택된 노드의 레벨이 0이 아닌 경우
		Btn_DeleteDir->Enabled = false; // "삭제" 버튼을 비활성화합니다.
		TV_ShereList->MultiSelect = true; // TreeView의 멀티 선택 기능을 활성화합니다.
	}
}


/*
* 멤버 함수: TV_ShereListEditing
* 이 함수는 TreeView에서 편집 이벤트가 발생했을 때 호출됩니다. 이 함수에서는 편집이 이루어지지 않도록 설정합니다.
* @param Sender: 편집 이벤트를 발생시킨 객체입니다.
* @param Node: 편집되려는 노드입니다.
* @param AllowEdit: 편집을 허용할지 여부를 결정하는 레퍼런스입니다.
*/
void __fastcall TMainForm::TV_ShereListEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit) {
	AllowEdit = false;
}

/*
* 멤버 함수: Btn_DeleteDirClick
* 이 함수는 "삭제" 버튼이 클릭되었을 때 호출됩니다. 선택된 노드와 그 하위 노드들을 삭제합니다.
* @param Sender: 버튼 클릭 이벤트를 발생시킨 객체입니다.
*/
void __fastcall TMainForm::Btn_DeleteDirClick(TObject *Sender) {
	// 선택된 노드가 없는 경우 함수를 종료합니다.
	if (TV_ShereList->Selected == NULL) return;

	if (TV_ShereList->Selected->Level == 0) { // 선택된 노드의 레벨이 0인 경우
		AnsiString confirmationMsg = TV_ShereList->Selected->Text + " 공유를 삭제 하시겠습니까?";
		// 사용자가 삭제를 확인하면 선택된 노드와 그 하위 노드들을 삭제하고, 관련 데이터도 제거합니다.
		if (MessageBoxA(this->Handle, confirmationMsg.c_str(), "확인", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			TTreeNode *RootNode = TV_ShereList->Selected; // 선택된 노드를 RootNode로 설정합니다.
			DeleteSubNodes(RootNode); // RootNode와 그 하위 노드들을 삭제합니다.

			// 관련 데이터를 제거합니다.
			Container->DeleteShereListFromName(RootNode->Text);
			Shere_ini->DeleteKey(INI_SHERE_LIST_NAME, RootNode->Text);
			Shere_ini->EraseSection(RootNode->Text);
			TV_ShereList->Selected->Delete(); // TreeView에서 선택된 노드를 삭제합니다.
		}
	}
}


/*
* 멤버 함수: DeleteSubNodes
* 이 함수는 주어진 루트 노드의 하위 노드를 모두 삭제합니다.
* @param RootNode: 하위 노드를 삭제할 루트 노드입니다.
*/
void __fastcall TMainForm::DeleteSubNodes(TTreeNode *RootNode) {
	for (int i = RootNode->Count - 1; i >= 0; --i) {
		TTreeNode *Node = RootNode->Item[i];
		// 레벨이 1인 노드를 삭제합니다.
		if (Node->Level == 1) {
			TShereItem *Item = (TShereItem*)Node->Data;
			Container->DeleteItem(Item->Name, Item);
			Shere_ini->DeleteKey(Node->Parent->Text, Node->Text);
			Node->Delete();
		}
	}
}

/*
* 멤버 함수: N1Click
* 이 함수는 "공유에 추가" 컨텍스트 메뉴가 클릭되었을 때 호출됩니다. 선택된 노드에 파일을 추가합니다.
* @param Sender: 메뉴 클릭 이벤트를 발생시킨 객체입니다.
*/
void __fastcall TMainForm::N1Click(TObject *Sender) {
	TTreeNode *Node = TV_ShereList->Selected;
	AddShereFile(Node);
}

/*
* 멤버 함수: AddShereFile
* 이 함수는 선택된 노드에 파일을 추가합니다.
* @param Node: 파일을 추가할 노드입니다.
*/
void __fastcall TMainForm::AddShereFile(TTreeNode *Node) {
	if (Node == NULL || Node->Level != 0)
		return;
	if (!OpenDialog1->Execute())
		return;
	// UI 업데이트 및 파일 처리
	LB_WorkName->Caption = "파일 추가 중...";
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
* 멤버 함수: ProcessFile
* 이 함수는 주어진 파일 경로의 파일을 처리합니다.
* @param filePath: 처리할 파일의 경로입니다.
* @param Node: 파일을 추가할 노드입니다.
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
* 멤버 함수: DeleteShere
* 이 함수는 TreeView에서 선택된 항목을 삭제합니다.
* @param Sender: 삭제 이벤트를 발생시킨 객체입니다.
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
* 멤버 함수: N5Click
* 이 함수는 "공유 삭제" 컨텍스트 메뉴가 클릭되었을 때 호출됩니다. 선택된 항목을 삭제합니다.
* @param Sender: 메뉴 클릭 이벤트를 발생시킨 객체입니다.
*/
void __fastcall TMainForm::N5Click(TObject *Sender) {
	DeleteShere(Sender);
}

/*
* 멤버 함수: TV_ShereListKeyDown
* 이 함수는 TreeView에서 키가 눌렸을 때 호출됩니다. Delete 키가 눌리면 선택된 항목을 삭제합니다.
* @param Sender: 키 다운 이벤트를 발생시킨 객체입니다.
* @param Key: 눌린 키의 코드입니다.
* @param Shift: Shift 키 상태입니다.
*/
void __fastcall TMainForm::TV_ShereListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
	if (Key == VK_DELETE) {
		TTreeView *treeView = (TTreeView*)Sender;
		if (treeView->SelectionCount >= 1) {
			int selectedCount = GetCountOfSelectedItems(treeView);
			AnsiString stTemp = IntToStr(selectedCount) + " 개의 항목을 삭제 하시겠습니까?";
			if (MessageBoxA(this->Handle, stTemp.c_str(), "확인", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				DeleteShere(Sender);
			}
		}
	}
}


/*
* 멤버 함수: Btn_ShereClick
* 이 함수는 "공유" 버튼이 클릭되었을 때 호출됩니다.
* 공유 중이면 공유를 중지하고, 그렇지 않으면 공유를 시작합니다.
* @param Sender: 버튼 클릭 이벤트를 발생시킨 객체입니다.
*/
void __fastcall TMainForm::Btn_ShereClick(TObject *Sender) {
	if (IS_Shering) { // 현재 공유 중인 경우
		StopSharing(); // 공유를 중지합니다.
	}
    else { // 공유 중이 아닌 경우
		StartSharing(); // 공유를 시작합니다.
    }
}

/*
* 멤버 함수: StartSharing
* 이 함수는 공유를 시작합니다.
* 공유 중인 상태를 나타내는 변수를 설정하고, 관련 UI를 비활성화합니다.
* 파일의 해시 처리를 수행하고, 공유 서버를 활성화합니다.
*/
void __fastcall TMainForm::StartSharing() {
	Btn_Shere->Caption = "공유 중지"; // 버튼의 레이블을 변경합니다.
	IS_Shering = true; // 공유 중인 상태를 나타내는 변수를 true로 설정합니다.

	// UI를 비활성화합니다.
	Btn_MakeDir->Enabled = false;
	Btn_DeleteDir->Enabled = false;
	TV_ShereList->Enabled = false;
	Btn_Shere->Enabled = false;

	if (HashingProce()) { // 파일의 해시 처리를 수행합니다. 성공하면 true를 반환합니다.
		// UI를 업데이트합니다.
		Btn_MakeDir->Enabled = false;
		Btn_DeleteDir->Enabled = false;
		TV_ShereList->Enabled = false;
		Btn_Shere->Enabled = true;

		Container->ClearSendingList(); // 전송 리스트를 초기화합니다.

		try {
			HTTPS->Active = true; // 공유 서버를 활성화합니다.
		}
		catch (...) { // 서버를 활성화하는 데 실패한 경우
			ShowMessage("공유 서버를 시작할 수 없습니다");
			Btn_ShereClick(NULL);
		}
	}
	else { // 해시 처리에 실패한 경우
		Btn_ShereClick(NULL);
	}
}

/*
* 멤버 함수: StopSharing
* 이 함수는 공유를 중지합니다.
* 공유 중인 상태를 나타내는 변수를 설정하고, 관련 UI를 활성화합니다.
* 공유 서버를 비활성화합니다.
*/
void __fastcall TMainForm::StopSharing() {
	Btn_Shere->Caption = "공유 시작"; // 버튼의 레이블을 변경합니다.
	IS_Shering = false; // 공유 중인 상태를 나타내는 변수를 false로 설정합니다.

	// UI를 활성화합니다.
	Btn_Shere->Enabled = true;
	Btn_MakeDir->Enabled = true;
	Btn_DeleteDir->Enabled = true;
	TV_ShereList->Enabled = true;

	try {
		HTTPS->Active = false; // 공유 서버를 비활성화합니다.
	}
	catch (...) { // 서버를 비활성화하는 데 실패한 경우
	}
}


/*
* 멤버 함수: HashingProce
* 이 함수는 전체 파일의 크기를 계산하고, 각 파일에 대해 MD5 해시를 계산하는 역할을 합니다.
* @return bool: 모든 파일의 해시 계산이 성공적으로 완료되면 true를, 그렇지 않으면 false를 반환합니다.
*/
bool __fastcall TMainForm::HashingProce() {

    /*
    * 멤버 변수: TotalFileSize
    * 이 변수는 처리할 파일들의 전체 크기를 저장합니다.
    */
    TotalFileSize = 0;

    /*
    * 멤버 변수: EndPos
    * 이 변수는 현재까지 처리한 파일의 크기를 저장합니다.
    */
    EndPos = 0;

    // 파일 목록을 순회하며 전체 파일 크기를 계산합니다.
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

    // 전체 파일 크기가 0 이하면 해시 계산을 진행할 수 없으므로 false를 반환합니다.
    if (TotalFileSize <= 0)
        return false;

    // 해시 계산 취소 플래그를 초기화합니다.
    IS_HashCencel = false;

    // 작업 타이머를 활성화하고, 작업 상태를 "파일 확인중"으로 표시합니다.
    WorkTimer->Enabled = true;
    LB_WorkName->Caption = "파일 확인중...";
    WorkForWait(true);
    PN_WorkProce->Visible = true;

    // 파일 목록을 순회하며 각 파일에 대해 MD5 해시를 계산합니다.
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

    // 해시 계산이 완료되었으므로 작업 타이머를 비활성화하고, 작업 상태를 숨깁니다.
    PN_WorkProce->Visible = false;
    WorkForWait(false);
    WorkTimer->Enabled = false;
    IS_HashCencel = false;

    // 모든 파일의 해시 계산이 성공적으로 완료되었으므로 true를 반환합니다.
    return true;
}

/*
* 멤버 함수: GetMD5HashProce
* 이 함수는 주어진 파일의 MD5 해시값을 계산합니다.
* @param FileName: MD5 해시값을 계산할 파일의 이름
* @return AnsiString: 계산된 MD5 해시값을 문자열로 반환합니다. 계산에 실패하면 빈 문자열을 반환합니다.
*/
AnsiString __fastcall TMainForm::GetMD5HashProce(AnsiString FileName) {

	// DWORD dwStatus = 0;
	BOOL bResult;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

    // 파일을 읽기 위한 핸들
	HANDLE hFile = NULL;
    // 파일의 내용을 읽어서 저장할 버퍼
	BYTE rgbFile[BUFSIZE];
    // 파일에서 읽은 바이트 수
	DWORD cbRead = 0;
    // MD5 해시값을 저장할 버퍼
	BYTE rgbHash[MD5LEN];
    // MD5 해시값의 길이
	DWORD cbHash = 0;

    // MD5 해시값을 저장할 문자열
	AnsiString MD5Hash = "";
     /*
    * 멤버 변수: hFile
    * 이 변수는 파일을 읽기 위한 핸들을 저장합니다.
	*/
    // 파일을 열기
	hFile = CreateFileA(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    // 파일 열기 실패 시
	if (INVALID_HANDLE_VALUE == hFile)
		return "";

    // 암호화 서비스 공급자 핸들 획득
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return "";

    // MD5 해시 객체 생성
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	try {
        // 파일의 내용을 읽어서 MD5 해시 계산
		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL)) {
            // 해시 계산 취소 시
			if (IS_HashCencel) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return "";
			}

            // 파일의 끝 도달 시
			if (cbRead == 0)
				break;
			EndPos += cbRead;
            // MD5 해시 데이터 추가
			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return "";
			}
            // UI 업데이트를 위해 메시지 처리
			Application->ProcessMessages();
		}
	}
	catch (Exception &E) {
		bResult = FALSE;
	}

    // 파일 읽기 실패 시
	if (!bResult) {
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return "";
	}

	cbHash = MD5LEN;
    // MD5 해시값 획득
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		AnsiString stTemp = "";
        // MD5 해시값을 문자열로 변환
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
	// MD5 해시값이 계산되면 파일명과 함께 저장
	if (MD5Hash != "")
		this->Shere_ini->WriteString(INI_FILE_HASH_DONE_NAME, FileName, MD5Hash);
	return MD5Hash;
}


void __fastcall TMainForm::TotalWorkProce() {
 // 전체 작업의 진행 상황을 백분율로 계산합니다.
    double progress = (double)EndPos / (double)TotalFileSize * 100;

    // 프로그레스 바의 위치를 업데이트합니다.
    PB_Total->Position = static_cast<int>(progress);

    // 진행 상황을 텍스트로 표시합니다.
	LB_Percent->Caption = FormatFloat("00.00 %", progress);
}

void __fastcall TMainForm::FormResize(TObject *Sender) {
	if (this->Width < 800)
		this->Width = 800;
}

// HTTPS GET 요청을 처리하는 함수
void __fastcall TMainForm::HTTPSCommandGet(TIdContext *AContext, TIdHTTPRequestInfo *ARequestInfo,
	TIdHTTPResponseInfo *AResponseInfo) {
    // favicon.ico 요청은 처리하지 않음
	if (ARequestInfo->Document == "/favicon.ico")
        return;

    // HTTP 요청 정보를 로그로 출력
	LogHTTPRequest(ARequestInfo);

    // UserAgent에 따라 요청 처리 방식 분기
    if (ARequestInfo->UserAgent != DOWNLOADER_USER_AGENT_STRING) {
        // 일반 요청 처리
        ProcessRegularRequest(ARequestInfo, AResponseInfo);
    } else {
        // 다운로더 요청 처리
        ProcessDownloaderRequest(ARequestInfo, AResponseInfo);
    }
}

// HTTP 요청 정보를 로그로 출력하는 함수
void __fastcall TMainForm::LogHTTPRequest(TIdHTTPRequestInfo *ARequestInfo) {
    // 각 정보를 로그로 출력
    Memo1->Lines->Add("UserAgent -> " + ARequestInfo->UserAgent);
    Memo1->Lines->Add("Command -> " + ARequestInfo->Command);
    Memo1->Lines->Add("Document -> " + ARequestInfo->Document);
    Memo1->Lines->Add("Params -> " + ARequestInfo->Params->Text);
    Memo1->Lines->Add("Ranges -> " + ARequestInfo->Range);
    Memo1->Lines->Add("Host -> " + ARequestInfo->Host);
}

// 일반 요청을 처리하는 함수
void __fastcall TMainForm::ProcessRegularRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo) {
    // 요청된 Document에 따라 처리 방식 분기
	if (ARequestInfo->Document == "/download") {
        // 파일을 웹으로 전송
        SendFileToWeb(AResponseInfo, Container, ARequestInfo->Params);
    } else if (ARequestInfo->Document == "/") {
        // 웹 본문 페이지 전송
        SendWebBodyPage(AResponseInfo, Container);
    } else {
        // 요청된 파일을 웹으로 전송
        SendRequestFileWeb(ARequestInfo->Document, AResponseInfo);
    }
}

// 다운로더 요청을 처리하는 함수
void __fastcall TMainForm::ProcessDownloaderRequest(TIdHTTPRequestInfo *ARequestInfo, TIdHTTPResponseInfo *AResponseInfo) {
    // HTTP 요청의 Command 타입에 따라 처리 방식 분기
    if (ARequestInfo->Command == "HEAD") {
        // HEAD 요청 시, 파일 크기 정보를 HTTP 헤더 정보로 전송
		SendFileSizeFromHTTPHeadInfo(AResponseInfo, Container, ARequestInfo->Params);
	} else if (ARequestInfo->Command == "GET") {
		// GET 요청 시, 요청된 Document에 따라 처리 방식 분기
		if(ARequestInfo->Document == "/shere_list") {
			// "/shere_list" 요청 시, 공유 리스트 정보를 전송
			SendShereList(AResponseInfo, Container);
		} else if (ARequestInfo->Document == "/shere_item_list") {
			// "/shere_item_list" 요청 시, 공유 아이템 리스트 정보를 전송
            SendShereItemList(AResponseInfo, Container, ARequestInfo->Params);
		} else if (ARequestInfo->Document == "/download") {
			// "/download" 요청 시, 요청된 파일을 클라이언트로 전송
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
