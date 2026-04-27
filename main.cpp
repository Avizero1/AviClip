//Code By Avizero https://github.com/Avizero1/AviClip
#include <vcl.h>
#pragma hdrstop
#include "main.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRM *FRM;
TStringList *FCFG=new TStringList;
AnsiString cfg, fe="AviClip.exe";
bool ClipReading = false, EditSGS=false, VisibleAPP;
//======================================================================================
//==========Мои функции
//Проверка повторного запуска
BOOL WeAreAlone(LPCTSTR szName)
	{ HANDLE hMutex=CreateMutex(NULL,TRUE,szName);
		if (GetLastError()==ERROR_ALREADY_EXISTS) {CloseHandle(hMutex); return 0;}
		return 1;
	}
//Чтение буфера обмена
AnsiString ClipBoardRead()
	{ wchar_t* string;  int cycle=10;
		do { try {	OpenClipboard(NULL);
								HANDLE buf=GetClipboardData(CF_UNICODETEXT);
								string = (wchar_t*)buf;
								CloseClipboard();
								if (buf==0) return NULL; else return string;
						 } catch (...) {}
				 Sleep(100);
			 } while (cycle--);
		return NULL;
	}
//Запись в буфер обмена
void ClipBoardWrite(AnsiString text)
	{ wchar_t * UniStr=new wchar_t[text.WideCharBufSize()];
		text.WideChar(UniStr,text.WideCharBufSize());
		int cycle=10;
		do { try {	OpenClipboard(NULL);
								EmptyClipboard();
								int k=2*wcslen(UniStr)+2;
								HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,k);
								memcpy(GlobalLock(hMem),UniStr,k);
								GlobalUnlock(hMem);
								SetClipboardData(CF_UNICODETEXT, hMem);
								CloseClipboard();
								delete [] UniStr;
								return ;
						 } catch (...) {}
					Sleep(100);
			 } while (cycle--);
		return ;
	}
//Сдвиг элементов TStringGrid списков на 1 вниз
void sdwig(unsigned int n,TStringGrid *SG)
	{	for (unsigned int i=n; i>0; i--) {SG->Cells[0][i]=SG->Cells[0][i-1];}}
//Включение/отключение кнопок StringGrid-Save
void ButtonOffSGSAVE()
	{ FRM->UPSGS->Enabled=false; FRM->DOWNSGS->Enabled=false;	FRM->EDIT->Enabled=false;}
void ButtonOnSGSAVE()
	{	if (FRM->SGSAVE->Row!=0) {FRM->UPSGS->Enabled=true;} else {FRM->UPSGS->Enabled=false;}
		if (FRM->SGSAVE->Row!=FRM->SGSAVE->RowCount-1)
			{FRM->DOWNSGS->Enabled=true;} else {FRM->DOWNSGS->Enabled=false;}
		FRM->EDIT->Enabled=true;
	}
//Сохранение настроек и данных в файл
void SaveToFile()
	{ if (EditSGS)
			{ EditSGS=false;
				FCFG->Clear();
				AnsiString Settings = (AnsiString)(FRM->Top)+";"+(AnsiString)(FRM->Left)+";"+(AnsiString)(FRM->Height)
															+";"+(AnsiString)(FRM->Width)+";"+(AnsiString)((int)(VisibleAPP))
															+";"+(FRM->FormStyle);
				FCFG->Append(Settings);
				for (unsigned int i=0; i<(unsigned)FRM->SGSAVE->RowCount; i++)
				{ FCFG->Append(StringReplace(FRM->SGSAVE->Cells[0][i], "\r\n", "<CR!LF>", TReplaceFlags() << rfReplaceAll));}
				FCFG->SaveToFile(cfg);
				FCFG->Clear();
			}
	}
//======================================================================================
//Отслеживание изменения буфера обмена и запись в StringGrid-History
void __fastcall TFRM::WMDrawClipboard(TWMDrawClipboard &Msg)
	{ if (PAUSPLAY->ImageIndex==0)
			{ ClipReading = true;
				try	{	AnsiString clipboard=ClipBoardRead();
							if (clipboard=="0" || clipboard=="\r\n") {clipboard="";}
							for (unsigned int i=0; i <=(unsigned)SGHISTORY->RowCount ; i++)
								{if (SGHISTORY->Cells[0][i]==clipboard)
									 {sdwig(i, SGHISTORY);SGHISTORY->Cells[0][0]="";}
								}
								if (SGHISTORY->Cells[0][0]!="")
									{ SGHISTORY->RowCount++;sdwig(SGHISTORY->RowCount, SGHISTORY);}
								SGHISTORY->Cells[0][0] = clipboard;
						} catch (...){}
				ClipReading = false;
			}
	}
//======================================================================================
//==========События StringGrid-History(правый)
//Установка актианой панелью SGHISTORY
void SGHISTORYActive() { FRM->MOVE->ImageIndex=2; FRM->SGHISTORY->SetFocus(); ButtonOffSGSAVE();}
void __fastcall TFRM::SGHISTORYClick(TObject *Sender)	{ SGHISTORYActive();}
void __fastcall TFRM::SGHISTORYEnter(TObject *Sender) { SGHISTORYActive();}
//Поместить текст в буфер обмена из выдел. ячейки
void __fastcall TFRM::SGHISTORYDblClick(TObject *Sender)
	{	if (SGHISTORY->Cells[0][SGHISTORY->Row]!="") {ClipBoardWrite(SGHISTORY->Cells[0][SGHISTORY->Row]);}}
//Окраска выделенной ячейки SGHISTORY(правый)
void __fastcall TFRM::SGHISTORYDrawCell(TObject *Sen,System::LongInt AC,System::LongInt AR,TRect &Rt,TGridDrawState St)
	{	if (SGHISTORY->Row == AR)
			{ if (MOVE->ImageIndex==2)	{	SGHISTORY->Canvas->Brush->Color=clSkyBlue;}
					else 	{ SGHISTORY->Canvas->Brush->Color=clWhite;}
				SGHISTORY->Canvas->Font->Color=SGHISTORY->Font->Color;
				SGHISTORY->Canvas->TextRect(Rt,Rt.Left+1,Rt.Top,SGHISTORY->Cells[AC][AR]);
			}
	}
//Очистка буфера обмена и его истории
void ClearClip()
	{ bool i = ((FRM->PAUSPLAY->ImageIndex==1)? false:true);
		if (i) {FRM->PAUSPLAY->ImageIndex=1;}
		ClipBoardWrite("");
		if (i) {FRM->PAUSPLAY->ImageIndex=0;}
		AnsiString Info="Clear History List?!?";
		if (MessageDlg(Info,mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo, 0) == mrYes)
			{	for ( long int i=FRM->SGHISTORY->RowCount; i>0 ; i-- )
				{ FRM->SGHISTORY->Cells[0][i]=""; FRM->SGHISTORY->RowCount--;}
				FRM->SGHISTORY->Cells[0][0]="";
			}
	}
void __fastcall TFRM::CLEARCLIPClick(TObject *Sender) { ClearClip();}
void __fastcall TFRM::CLEARCLIPBOARDClick(TObject *Sender)  {	ClearClip();}
//======================================================================================
//==========События StringGrid-Save(левый)
//Установка_актианой_панелью-SGSAVE
void SGSAVEActive()
	{ FRM->MOVE->ImageIndex=3; FRM->SGSAVE->SetFocus();
		if (FRM->SGSAVE->RowCount<2) { ButtonOffSGSAVE(); FRM->EDIT->Enabled=true;}
			else { ButtonOnSGSAVE();}
	}
void __fastcall TFRM::SGSAVEClick(TObject *Sender)	{	SGSAVEActive();}
void __fastcall TFRM::SGSAVEEnter(TObject *Sender)	{	SGSAVEActive();}
//Окраска-SGSAVE(левый)
void __fastcall TFRM::SGSAVEDrawCell(TObject *Sen,System::LongInt AC,System::LongInt AR,TRect &Rt,TGridDrawState St)
	{ if (SGSAVE->Row == AR )
			{ if (MOVE->ImageIndex==3) {	SGSAVE->Canvas->Brush->Color=clSkyBlue;}
					else { SGSAVE->Canvas->Brush->Color=clWhite;}
				SGSAVE->Canvas->Font->Color=SGSAVE->Font->Color;
				SGSAVE->Canvas->TextRect(Rt,Rt.Left+1,Rt.Top,SGSAVE->Cells[AC][AR]);
			}
	}
//Поднять выделенный элемент списка
void __fastcall TFRM::UPSGSClick(TObject *Sender)
	{ if (SGSAVE->RowCount>1 && SGSAVE->Row!=0)
			{	AnsiString cellsA=SGSAVE->Cells[0][SGSAVE->Row], cellsB=SGSAVE->Cells[0][SGSAVE->Row-1];
				SGSAVE->Cells[0][SGSAVE->Row-1]=cellsA; SGSAVE->Cells[0][SGSAVE->Row]=cellsB;
				SGSAVE->Row--;
				EditSGS=true;
			}
	}
//Опустить выделенный элемент списка
void __fastcall  TFRM::DOWNSGSClick(TObject *Sender)
	{ if (SGSAVE->RowCount>1 && SGSAVE->Row!=SGSAVE->RowCount-1)
			{	AnsiString cellsA=SGSAVE->Cells[0][SGSAVE->Row], cellsB=SGSAVE->Cells[0][SGSAVE->Row+1];
				SGSAVE->Cells[0][SGSAVE->Row+1]=cellsA; SGSAVE->Cells[0][SGSAVE->Row]=cellsB;
				SGSAVE->Row++;
				EditSGS=true;
			}
	}
//========== Режим редактора
void ResizeREDIT()
	{	FRM->REDIT->Top=60;	FRM->REDIT->Height=FRM->ClientHeight-65; FRM->REDIT->Width=FRM->ClientWidth-10;}
void EditMode(bool mod)
	{ FRM->UPSGS->Visible=!mod; FRM->DOWNSGS->Visible=!mod; FRM->EDIT->Visible=!mod; FRM->PAUSPLAY->Visible=!mod;
		FRM->MOVE->Visible=!mod; FRM->DELIT->Visible=!mod; FRM->CLEARCLIP->Visible=!mod;
		FRM->SGSAVE->Visible=!mod; FRM->SGHISTORY->Visible=!mod;
		FRM->REDIT->Visible=mod; FRM->SAVEEDIT->Visible=mod; FRM->NOTSAVEEDIT->Visible=mod;
	}
//Вход в редактор
void __fastcall TFRM::EDITClick(TObject *Sender)
	{ REDIT->Text=SGSAVE->Cells[0][SGSAVE->Row]; ResizeREDIT(); EditMode(true);}
//Выход из редактора	
void __fastcall TFRM::SAVEEDITClick(TObject *Sender)
	{ SGSAVE->Cells[0][SGSAVE->Row]=REDIT->Text; REDIT->Text=""; EditMode(false); SGSAVE->SetFocus(); EditSGS=true;}
void __fastcall TFRM::NOTSAVEEDITClick(TObject *Sender) { EditMode(false); SGSAVE->SetFocus(); REDIT->Text="";}
//======================================================================================
//==========Внешние кнопки
void __fastcall TFRM::MOVEClick(TObject *Sender)
{ if (MOVE->ImageIndex==2)
		{  if (SGHISTORY->Cells[0][SGHISTORY->Row]!="" && SGSAVE->Cells[0][0]!=SGHISTORY->Cells[0][SGHISTORY->Row])
				{	for (unsigned int i=0; i <(unsigned)SGSAVE->RowCount ; i++)
					{	if (SGSAVE->Cells[0][i]==SGHISTORY->Cells[0][SGHISTORY->Row])
							{	sdwig(i, SGSAVE); SGSAVE->Cells[0][0]="";}
					}
				if (SGSAVE->Cells[0][0]!="") { SGSAVE->RowCount++; sdwig(SGSAVE->RowCount, SGSAVE);}
				SGSAVE->Cells[0][0] = SGHISTORY->Cells[0][SGHISTORY->Row];
        EditSGS=true;
			}
		} else
			{ if (SGSAVE->Cells[0][SGSAVE->Row]!="")
					{ bool i;
						i = ((FRM->PAUSPLAY->ImageIndex==1)? true:false);
						if (i) {PAUSPLAY->ImageIndex=0;}
						ClipBoardWrite(SGSAVE->Cells[0][SGSAVE->Row]);
						if (i) {PAUSPLAY->ImageIndex=1;}
					}
			}
}
//Пауза и плей(продолжение отслежива буфера обмена)
void PausPlayMode()
	{	if (FRM->PAUSPLAY->ImageIndex==0)
		{	FRM->PAUSPLAY->ImageIndex=1; FRM->PAUSPLAY->Caption="Play";
			FRM->PLAY->Visible=true; FRM->PAUSE->Visible=false;
		}	else
				{	FRM->PAUSPLAY->ImageIndex=0; FRM->PAUSPLAY->Caption="Pause";
					FRM->PLAY->Visible=false; FRM->PAUSE->Visible=true;
				}
	}
void __fastcall TFRM::PAUSPLAYClick(TObject *Sender) { PausPlayMode();}
void __fastcall TFRM::PLAYClick(TObject *Sender)	{ PausPlayMode();}
void __fastcall TFRM::PAUSEClick(TObject *Sender)	{	PausPlayMode();}
//Удаление выделенного элемента активного списка 
void __fastcall TFRM::DELITClick(TObject *Sender)
 { TStringGrid *focus;
	 if (FRM->MOVE->ImageIndex==2) {focus=SGHISTORY;} else {focus=SGSAVE;}
	 for ( long int i=focus->Row; i<=focus->RowCount ; i++ ) {focus->Cells[0][i]=focus->Cells[0][i+1];}
	 focus->Cells[0][focus->RowCount]="";
	 focus->RowCount--;
	 if (focus==FRM->SGSAVE) { ButtonOnSGSAVE(); EditSGS=true;}
 }
//Скрытие/не скрытие формы при запуске программы
void __fastcall TFRM::HIDEAPPClick(TObject *Sender)
	{ if (HIDEAPP->ImageIndex==4)
			{ VisibleAPP=false; HIDEAPP->ImageIndex=5;}
			else { VisibleAPP=true; HIDEAPP->ImageIndex=4;}
		EditSGS=true;
	}
//Режим работы окна программы(всегда на переднем плане/норм. режим)
void __fastcall TFRM::MODEFORMAPPClick(TObject *Sender)
	{	if (	MODEFORMAPP->ImageIndex==4 )
			{ MODEFORMAPP->ImageIndex=5; FRM->FormStyle=fsStayOnTop;}
			else { MODEFORMAPP->ImageIndex=4; FRM->FormStyle=fsNormal; }
		EditSGS=true;
	}
//======================================================================================
void __fastcall TFRM::TIMERTimer(TObject *Sender) { SaveToFile();}
//======================================================================================
//==========События Формы
//Создание программы
__fastcall TFRM::TFRM(TComponent* Owner) : TForm(Owner)
	{ cfg = StringReplace(ParamStr(0),".exe",".cfg",TReplaceFlags()<<rfReplaceAll);
		FNextCbViewer=SetClipboardViewer(this->Handle);
		//Проверка названия программы
		if (ParamStr(0).SubString(ParamStr(0).Length()-10,11) != fe)
		 { ShowMessage("!The program name should be AviClip.exe!"); exit(0);}
		//Проверка повторного запуска
		if (!WeAreAlone((wchar_t*)(fe.c_str())))
			{ShowMessage("!The program has already been launched!"); exit(0);}
	}
//Создание окна программы
void __fastcall TFRM::FRMCreate(TObject *Sender)
	{ Application->OnMinimize = AppMinimize;
		if (FileExists(cfg))
			{ TRAYICO->Visible=true; FRM->Visible=true; SGHISTORY->SetFocus();
				//Чтение файла с настройками и данными
				try { FCFG->LoadFromFile(cfg);
							if (FCFG->Count > 0)
								{ for (unsigned int i=1; i<(unsigned)FCFG->Count; i++)
										{ SGSAVE->Cells[0][i-1] = StringReplace(FCFG->Strings[i], "<CR!LF>", "\r\n", TReplaceFlags()<<rfReplaceAll);
											if (i!=1)	{SGSAVE->RowCount++;}
										}
									//Установка настроек программы
									AnsiString Settings=FCFG->Strings[0]; char *s = ";";
									FRM->Top       = atoi(strtok( Settings.c_str(), s));
									FRM->Left      = atoi(strtok( NULL, s));
									FRM->Height    = atoi(strtok( NULL, s));
									FRM->Width     = atoi(strtok( NULL, s));
									if (  atoi(strtok( NULL, s))==0 )
										{ VisibleAPP=false; HIDEAPP->ImageIndex=5; FRM->Hide();}
										else { VisibleAPP=true; HIDEAPP->ImageIndex=4;}
									if (	atoi(strtok( NULL, s))==0 )
										{MODEFORMAPP->ImageIndex=4; FRM->FormStyle=fsNormal; }
										else { MODEFORMAPP->ImageIndex=5; FRM->FormStyle=fsStayOnTop;}
									FCFG->Clear();
								}
						}catch (...) {ShowMessage("!Config not available!");}
			} else
				{ //Вывод пользовательского соглашения
					AnsiString Info="By accepting this user agreement, you accept the following aspects:\r\n"
													"1) Avizero has unconditional rights to this program;\r\n"
													"2) The author is not responsible for the functioning the program when making code changes;\r\n"
													"3) The product is provided to the user in its original form, so it may have errors;\r\n"
													"4) The user is not allowed to modify any component in the program library.\r\n";
					if (MessageDlg(Info,mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo, 0) == mrYes)
						{	TRAYICO->Visible=true; FRM->Visible=true; SGHISTORY->SetFocus();}
						else {ShowMessage("!GOODBYE USER!"); exit(0);}
					VisibleAPP=true; HIDEAPP->ImageIndex=4;
					MODEFORMAPP->ImageIndex=4; FRM->FormStyle=fsNormal;
				}
		TIMER->Enabled=true;
	}
//Измененние размера окна программы
void __fastcall TFRM::FormResize(TObject *Sender)
	{ int sgNewWidth=(FRM->ClientWidth-135)/2, NewHeight=FRM->ClientHeight-10, buttonLeft=sgNewWidth+70;
		SGSAVE->Width=sgNewWidth;	SGSAVE->DefaultColWidth=sgNewWidth; SGSAVE->Height=NewHeight;
		SGHISTORY->Width=sgNewWidth; SGHISTORY->DefaultColWidth=sgNewWidth; SGHISTORY->Left=sgNewWidth+130;
		SGHISTORY->Height=NewHeight;
		PAUSPLAY->Left=buttonLeft; MOVE->Left=buttonLeft; DELIT->Left=buttonLeft; CLEARCLIP->Left=buttonLeft;
		if (REDIT->Visible)	{ ResizeREDIT();}
		EditSGS=true;
	}
//Сворачивание программы в трей
void __fastcall TFRM::AppMinimize(TObject *Sender) 	{ FRM->Visible=false; EditMode(false); }
//Возврат окна в видемый режим
void ShowApp()
	{ FRM->Visible=true;
		FRM->WindowState=wsNormal;
		if (FRM->FormStyle==fsNormal) { FRM->FormStyle=fsStayOnTop; FRM->FormStyle=fsNormal;}
	}
void __fastcall TFRM::SHOWAPPClick(TObject *Sender)	{ ShowApp();}
void __fastcall TFRM::TRAYICOClick(TObject *Sender) { ShowApp();}
//Завершение работы программы
void __fastcall TFRM::FormDestroy(TObject *Sender)
	{ if (FNextCbViewer)	{	ChangeClipboardChain(this->Handle,FNextCbViewer); FNextCbViewer=0;}}
void __fastcall TFRM::FormClose(TObject *Sender, TCloseAction &Action) { SaveToFile();}
void __fastcall TFRM::EXITClick(TObject *Sender) { SaveToFile(); exit(0);}