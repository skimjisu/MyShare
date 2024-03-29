

#ifndef SystemIconProceH
#define SystemIconProceH

#include <System.Classes.hpp>
#include <System.Contnrs.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ImgList.hpp>

class TIconItem : public TObject {
public:
	AnsiString Ext;
	int IconIdex;
};

class TExtIconList : public TObject {
public:

	TImageList *IconList;
	TObjectList *IconInfoList;

	__fastcall TExtIconList(){
		this->IconInfoList = new TObjectList;
		this->IconList = new TImageList(NULL);
		this->IconList->ColorDepth = cd32Bit;
		AddExtSystemIcon("sdlfjsdlkfjssakdjasldksalakd");

	};

	int __fastcall IsInExtSystemIcon(AnsiString FileExt);
	int __fastcall AddExtSystemIcon(AnsiString Ext);
	int __fastcall AddCustomSystemIcon(AnsiString Ext,TIcon *AddIcon);
};

#endif
