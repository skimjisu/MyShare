#include <vcl.h>
#pragma hdrstop

#include "fmMakeDir.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TMakeDirForm *MakeDirForm;

/*
* ������: TMakeDirForm
* �� �����ڴ� ���� Ŭ������ �ν��Ͻ��� �ʱ�ȭ�մϴ�.
* @param Owner: �� ���� �����ڸ� ��Ÿ���� ������Ʈ�Դϴ�.
*/
__fastcall TMakeDirForm::TMakeDirForm(TComponent* Owner)	: TForm(Owner){
}

/*
* �̺�Ʈ �ڵ鷯: FormCreate
* �� �Լ��� ���� ������ �� ȣ��Ǹ�, IS_OK ������ false�� �ʱ�ȭ�մϴ�.
* @param Sender: �� �̺�Ʈ�� Ʈ������ ��ü�Դϴ�.
*/
void __fastcall TMakeDirForm::FormCreate(TObject *Sender){
	IS_OK = false;
}

/*
* �̺�Ʈ �ڵ鷯: Button1Click
* �� �Լ��� ��ư 1�� Ŭ���� �� ȣ��Ǹ�, IS_OK ������ true�� �����ϰ� ���� �ݽ��ϴ�.
* @param Sender: �� �̺�Ʈ�� Ʈ������ ��ü�Դϴ�.
*/
void __fastcall TMakeDirForm::Button1Click(TObject *Sender){
	IS_OK = true;
	this->Close();
}

/*
* �̺�Ʈ �ڵ鷯: Button2Click
* �� �Լ��� ��ư 2�� Ŭ���� �� ȣ��Ǹ�, IS_OK ������ false�� �����ϰ� ���� �ݽ��ϴ�.
* @param Sender: �� �̺�Ʈ�� Ʈ������ ��ü�Դϴ�.
*/
void __fastcall TMakeDirForm::Button2Click(TObject *Sender){
	IS_OK = false;
	this->Close();
}

/*
* �̺�Ʈ �ڵ鷯: Edit1KeyPress
* �� �Լ��� �ؽ�Ʈ �ڽ��� Ű�� �Էµ� �� ȣ��˴ϴ�. ����Ű�� �ԷµǸ� Button1Click��, ESC Ű�� �ԷµǸ� Button2Click�� ȣ���մϴ�.
* @param Sender: �� �̺�Ʈ�� Ʈ������ ��ü�Դϴ�.
* @param Key: �Էµ� Ű�� ��Ÿ���� �����Դϴ�.
*/
void __fastcall TMakeDirForm::Edit1KeyPress(TObject *Sender, System::WideChar &Key){
	if(Key == 13)Button1Click(this);
	if(Key == 27){
		Key = 0;
		Button2Click(this);
	}
}
