//Code By Avizero
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("main.cpp", FRM);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFRM), &FRM);
    Application->ShowMainForm = false;
		Application->Run();
	}
	catch (Exception &exception) {}
	return 0;
}
//---------------------------------------------------------------------------
