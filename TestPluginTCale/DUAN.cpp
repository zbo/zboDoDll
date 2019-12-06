#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

__declspec(dllexport) BI* CreateBI(FXing* FX1, FXing* FX2)
{
	BI* bi = new BI;
	bi->FirstFX = FX1;
	bi->SecondFX = FX2;
	bi->Start = FX1->Second->i;
	bi->End = FX2->Second->i;
	if (FX1->FxType == FX2->FxType) {
		throw std::logic_error("Same FX Type connected");
	}
	if (FX1->FxType == FXing::Ding && FX2->FxType == FXing::Di) {
		bi->BITpye = BI::XIA;
		bi->High = FX1->Second->High;
		bi->Low = FX2->Second->Low;
	}
	if (FX1->FxType == FXing::Di && FX2->FxType == FXing::Ding) {
		bi->BITpye = BI::SHANG;
		bi->High = FX2->Second->High;
		bi->Low = FX1->Second->Low;
	}
	return bi;
}

__declspec(dllexport) vector<BI*> GenerateBIVector(vector<FXing*> Final_FXVector)
{
	vector<BI*> BIVector;
	int first_Index = 0;
	int second_Index = 1;
	if (Final_FXVector.size() >= 2) {
		while (second_Index < Final_FXVector.size()) {
			FXing* FX1 = Final_FXVector[first_Index];
			FXing* FX2 = Final_FXVector[second_Index];
			BI* bi = CreateBI(FX1, FX2);
			BIVector.push_back(bi);
			first_Index++;
			second_Index++;
		}
	}
	return BIVector;
}

__declspec(dllexport) vector<BI*> GenerateTZXL_Shang(vector<BI*> BIVector)
{
	vector<BI*> TZXL_Shang;
	for (int i = 0; i < BIVector.size(); i++)
	{
		if (BIVector[i]->BITpye == BI::SHANG) {
			TZXL_Shang.push_back(BIVector[i]);
		}
	}
	return TZXL_Shang;
}

__declspec(dllexport) vector<BI*> GenerateTZXL_Xia(vector<BI*> BIVector)
{
	vector<BI*> TZXL_Xia;
	for (int i = 0; i < BIVector.size(); i++)
	{
		if (BIVector[i]->BITpye == BI::XIA) {
			TZXL_Xia.push_back(BIVector[i]);
		}
	}
	return TZXL_Xia;
}

__declspec(dllexport) bool Valid_TZDIFx(BI* Bi_First, BI* Bi_Second, BI* Bi_Third)
{
	bool cond1 = Bi_First->High > Bi_Second->High&& Bi_First->Low > Bi_Second->Low;
	bool cond2 = Bi_Third->High > Bi_Second->High&& Bi_Third->Low > Bi_Second->Low;
	return cond1 && cond2;
}

__declspec(dllexport) bool Valid_TZDINGFx(BI* Bi_First, BI* Bi_Second, BI* Bi_Third)
{
	bool cond1 = Bi_First->High < Bi_Second->High && Bi_First->Low < Bi_Second->Low;
	bool cond2 = Bi_Third->High < Bi_Second->High && Bi_Third->Low < Bi_Second->Low;
	return cond1 && cond2;
}

__declspec(dllexport) TZXLFXing* Find_TZXL_DI_FX(vector<BI*> TZXL_Shang, int from)
{
	TZXLFXing* TZFX = new TZXLFXing;
	TZFX->Type = TZXLFXing::DI;
	TZFX->Valid = false;
	for (int i = 1 + from; i < TZXL_Shang.size() - 1; i++) {
		if (Valid_TZDIFx(TZXL_Shang[i - 1], TZXL_Shang[i], TZXL_Shang[i + 1])) {
			TZFX->First = TZXL_Shang[i - 1];
			TZFX->Second = TZXL_Shang[i];
			TZFX->Third = TZXL_Shang[i + 1];
			TZFX->Search_Index = i;
			TZFX->Valid = true;
			return TZFX;
		}
	}
	return TZFX;
}

__declspec(dllexport) TZXLFXing* Find_TZXL_DING_FX(vector<BI*> TZXL_Xia, int from)
{
	TZXLFXing* TZFX = new TZXLFXing;
	TZFX->Type = TZXLFXing::DING;
	TZFX->Valid = false;
	for (int i = 1 + from; i < TZXL_Xia.size() - 1; i++) {
		if (Valid_TZDINGFx(TZXL_Xia[i - 1], TZXL_Xia[i], TZXL_Xia[i + 1])) {
			TZFX->First = TZXL_Xia[i - 1];
			TZFX->Second = TZXL_Xia[i];
			TZFX->Third = TZXL_Xia[i + 1];
			TZFX->Search_Index = i;
			TZFX->Valid = true;
			return TZFX;
		}
	}
	return TZFX;
}

__declspec(dllexport) vector<TZXLFXing*> Ordered_TZXL_FX(vector<TZXLFXing*> TZXL_Shang_Vector, vector<TZXLFXing*> TZXL_Xia_Vector)
{
	vector<TZXLFXing*> Ordered_TZXL_Vector;
	TZXLFXing* FX1 = new TZXLFXing;
	while (TZXL_Shang_Vector.size() > 0 || TZXL_Xia_Vector.size() > 0) {
		if (TZXL_Shang_Vector.size() == 0 && TZXL_Xia_Vector.size() != 0) {
			FX1 = TZXL_Xia_Vector[0];
			TZXL_Xia_Vector.erase(std::begin(TZXL_Xia_Vector));
			Ordered_TZXL_Vector.push_back(FX1);
		}
		if (TZXL_Shang_Vector.size() != 0 && TZXL_Xia_Vector.size() == 0) {
			FX1 = TZXL_Shang_Vector[0];
			TZXL_Shang_Vector.erase(std::begin(TZXL_Shang_Vector));
			Ordered_TZXL_Vector.push_back(FX1);
		}
		if (TZXL_Shang_Vector.size() != 0 && TZXL_Xia_Vector.size() != 0) {
			bool s = TZXL_Shang_Vector[0]->Second->Start < TZXL_Xia_Vector[0]->Second->Start;
			if (s) {
				FX1 = TZXL_Shang_Vector[0];
				TZXL_Shang_Vector.erase(std::begin(TZXL_Shang_Vector));
			}
			else {
				FX1 = TZXL_Xia_Vector[0];
				TZXL_Xia_Vector.erase(std::begin(TZXL_Xia_Vector));
			}
			Ordered_TZXL_Vector.push_back(FX1);
		}
	}
	return Ordered_TZXL_Vector;
}
__declspec(dllexport) vector<TZXLFXing*> Generate_TZXL_Shang_FX_Vector(vector<BI*> TZXL_Shang)
{
	vector<TZXLFXing*> TZXL_Shang_Vector;
	TZXLFXing* TZ_FX_Shang;
	int from = 0;
	do {
		TZ_FX_Shang = Find_TZXL_DI_FX(TZXL_Shang, from);
		if (TZ_FX_Shang->Valid) {
			TZXL_Shang_Vector.push_back(TZ_FX_Shang);
			from = TZ_FX_Shang->Search_Index;
		}
	} while (TZ_FX_Shang->Valid);
	return TZXL_Shang_Vector;
}

__declspec(dllexport) vector<TZXLFXing*> Generate_TZXL_Xia_FX_Vector(vector<BI*> TZXL_Xia)
{
	vector<TZXLFXing*> TZXL_Xia_Vector;
	TZXLFXing* TZ_FX_Xia;
	int from = 0;
	do {
		TZ_FX_Xia = Find_TZXL_DING_FX(TZXL_Xia, from);
		if (TZ_FX_Xia->Valid) {
			TZXL_Xia_Vector.push_back(TZ_FX_Xia);
			from = TZ_FX_Xia->Search_Index;
		}
	} while (TZ_FX_Xia->Valid);
	return TZXL_Xia_Vector;
}