#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

USEFORM("fmMain.cpp", MainForm);
USEFORM("fmSet.cpp", Set_Form);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int){

	if( IsDebuggerPresent() )return 0;
	HANDLE hMutex = CreateMutexA(NULL, true, "MyShere Client Project");
	if (hMutex == NULL || GetLastError() != 0)return 0;

	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TSet_Form), &Set_Form);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}

