#include <vcl.h>
#pragma hdrstop

#include "fmMakeDir.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TMakeDirForm *MakeDirForm;

/*
* 생성자: TMakeDirForm
* 이 생성자는 현재 클래스의 인스턴스를 초기화합니다.
* @param Owner: 이 폼의 소유자를 나타내는 컴포넌트입니다.
*/
__fastcall TMakeDirForm::TMakeDirForm(TComponent* Owner)	: TForm(Owner){
}

/*
* 이벤트 핸들러: FormCreate
* 이 함수는 폼이 생성될 때 호출되며, IS_OK 변수를 false로 초기화합니다.
* @param Sender: 이 이벤트를 트리거한 객체입니다.
*/
void __fastcall TMakeDirForm::FormCreate(TObject *Sender){
	IS_OK = false;
}

/*
* 이벤트 핸들러: Button1Click
* 이 함수는 버튼 1이 클릭될 때 호출되며, IS_OK 변수를 true로 설정하고 폼을 닫습니다.
* @param Sender: 이 이벤트를 트리거한 객체입니다.
*/
void __fastcall TMakeDirForm::Button1Click(TObject *Sender){
	IS_OK = true;
	this->Close();
}

/*
* 이벤트 핸들러: Button2Click
* 이 함수는 버튼 2가 클릭될 때 호출되며, IS_OK 변수를 false로 설정하고 폼을 닫습니다.
* @param Sender: 이 이벤트를 트리거한 객체입니다.
*/
void __fastcall TMakeDirForm::Button2Click(TObject *Sender){
	IS_OK = false;
	this->Close();
}

/*
* 이벤트 핸들러: Edit1KeyPress
* 이 함수는 텍스트 박스에 키가 입력될 때 호출됩니다. 엔터키가 입력되면 Button1Click을, ESC 키가 입력되면 Button2Click을 호출합니다.
* @param Sender: 이 이벤트를 트리거한 객체입니다.
* @param Key: 입력된 키를 나타내는 문자입니다.
*/
void __fastcall TMakeDirForm::Edit1KeyPress(TObject *Sender, System::WideChar &Key){
	if(Key == 13)Button1Click(this);
	if(Key == 27){
		Key = 0;
		Button2Click(this);
	}
}
