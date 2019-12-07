// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdio.h>
#include <tchar.h>

class KXian{
public:
	float High;
	float Low;
	int i;
	bool BHan;
	int Period;
};

class FXing{
public:
	enum DINGDI {Ding, Di};
	KXian* First;
	KXian* Second;
	KXian* Third;
	DINGDI FxType;
	float get_max_gap() {
		float gap1 = 0;
		float gap2 = 0;
		if (this->FxType == Ding) {
			gap1 = this->Second->Low - this->First->High;
			gap2 = this->Second->Low - this->Third->High;
			return gap1 > gap2 ? gap1 : gap2;
		}
		else {
			gap1 = this->First->Low-this->Second->High;
			gap2 = this->Third->Low - this->Second->High;
			return gap1 > gap2 ? gap1 : gap2;
		}
	};
	bool contain_gap() {
		float gap = this->get_max_gap();
		return gap*9 > this->Second->High;
	}
};

class BI {
public:
	enum FANGXIANG { SHANG, XIA };
	FXing* FirstFX;
	FXing* SecondFX;
	FANGXIANG BITpye;
	float High;
	float Low;
	int Start;
	int End;
};

class TZXLFXing{
public:
	enum TZXLFXingType {DING, DI};
	BI* First;
	BI* Second;
	BI* Third;
	bool Valid;
	int Search_Index;
	TZXLFXingType Type;
};

class SearchDuan {
public:
	enum DuanDirection {Shang,Xia};
	//vector<BI*>* TeZhengXL;
	//vector<BI*>* ZhuXL;
	BI* Start;
	BI* End;
	DuanDirection Type;
	bool Valid;
};

struct FXSearchResult {
	FXing* FirstFX;
	FXing* SecondFX;
	int FirstFX_Index;
	int SecondFX_Index;
	bool FirstFX_Confirmed;
};

struct BaoHanRela{
	enum BaoHanType {QianDa,HouDa,No};
	bool isBaoHan;
	BaoHanType BHType; //first BH second 1, -1, 0
};

// TODO: 在此处引用程序需要的其他头文件
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1EDD32FF_DA00_4490_B90B_36DCFEF9AB45__INCLUDED_)
#define AFX_STDAFX_H__1EDD32FF_DA00_4490_B90B_36DCFEF9AB45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1EDD32FF_DA00_4490_B90B_36DCFEF9AB45__INCLUDED_)
