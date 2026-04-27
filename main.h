//Code By Avizero
//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFRM : public TForm
{
__published:	// IDE-managed Components
//Component Form - FRM
	TPopupMenu *POPMENU;
	TTrayIcon *TRAYICO;
	TImageList *IMAGLIST;
	TStringGrid *SGSAVE;
	TStringGrid *SGHISTORY;
	TSpeedButton *PAUSPLAY;
	TSpeedButton *MOVE;
	TSpeedButton *EDIT;
	TSpeedButton *DELIT;
	TSpeedButton *UPSGS;
	TSpeedButton *DOWNSGS;
	TSpeedButton *CLEARCLIP;
	TSpeedButton *SAVEEDIT;
	TSpeedButton *NOTSAVEEDIT;
	TRichEdit *REDIT;
	TMenuItem *PAUSE;
	TMenuItem *PLAY;
	TMenuItem *CLEARCLIPBOARD;
	TMenuItem *SHOWAPP;
	TTimer *TIMER;
	TMenuItem *EXIT;
	TMainMenu *SETMENU;
	TMenuItem *HIDEAPP;
	TMenuItem *MODEFORMAPP;
	TMenuItem *Options;
	void __fastcall FRMCreate(TObject *Sender);
	void __fastcall SGHISTORYClick(TObject *Sender);
	void __fastcall SGHISTORYEnter(TObject *Sender);
	void __fastcall SGSAVEClick(TObject *Sender);
	void __fastcall SGSAVEEnter(TObject *Sender);
	void __fastcall MOVEClick(TObject *Sender);
	void __fastcall SGHISTORYDblClick(TObject *Sender);
	void __fastcall PAUSPLAYClick(TObject *Sender);
	void __fastcall UPSGSClick(TObject *Sender);
	void __fastcall DOWNSGSClick(TObject *Sender);
	void __fastcall DELITClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CLEARCLIPClick(TObject *Sender);
	void __fastcall EDITClick(TObject *Sender);
	void __fastcall SAVEEDITClick(TObject *Sender);
	void __fastcall NOTSAVEEDITClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall PLAYClick(TObject *Sender);
	void __fastcall PAUSEClick(TObject *Sender);
	void __fastcall CLEARCLIPBOARDClick(TObject *Sender);
	void __fastcall SHOWAPPClick(TObject *Sender);
	void __fastcall TRAYICOClick(TObject *Sender);
	void __fastcall AppMinimize(TObject *Sender);
	void __fastcall SGSAVEDrawCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall SGHISTORYDrawCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall EXITClick(TObject *Sender);
	void __fastcall TIMERTimer(TObject *Sender);
	void __fastcall HIDEAPPClick(TObject *Sender);
	void __fastcall MODEFORMAPPClick(TObject *Sender);
private:	// User declarations
//Events ClipBoard
	HWND FNextCbViewer;
	void __fastcall WMDrawClipboard(TWMDrawClipboard &Msg);
public:		// User declarations
	__fastcall TFRM(TComponent* Owner);

// ClipBoard  read
	BEGIN_MESSAGE_MAP
	VCL_MESSAGE_HANDLER(WM_DRAWCLIPBOARD, TWMDrawClipboard, WMDrawClipboard)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFRM *FRM;
//---------------------------------------------------------------------------
#endif
