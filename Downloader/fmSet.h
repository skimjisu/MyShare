//---------------------------------------------------------------------------

#ifndef fmSetH
#define fmSetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSet_Form : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TEdit *Dir_ED;
	TSpeedButton *Dir_FD;
	TLabel *Dir_LB;
private:	// User declarations
public:		// User declarations
	__fastcall TSet_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSet_Form *Set_Form;
//---------------------------------------------------------------------------
#endif
