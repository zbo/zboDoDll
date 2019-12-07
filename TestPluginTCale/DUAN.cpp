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
		}
		if (TZXL_Shang_Vector.size() != 0 && TZXL_Xia_Vector.size() == 0) {
			FX1 = TZXL_Shang_Vector[0];
			TZXL_Shang_Vector.erase(std::begin(TZXL_Shang_Vector));
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
		}
		bool exist = false;
		for (int i = 0; i < Ordered_TZXL_Vector.size(); i++) {
			if (Ordered_TZXL_Vector[i]->Second->Start == FX1->Second->Start)
				exist = true;
		}
		if(!exist)
			Ordered_TZXL_Vector.push_back(FX1);
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

__declspec(dllexport) BaoHanRela BaoHanTZXL(BI* firstBI, BI* secondBI) {
	BaoHanRela rela;
	if (firstBI->High >= secondBI->High && firstBI->Low <= secondBI->Low) {
		rela.isBaoHan = true;
		rela.BHType = BaoHanRela::QianDa;
		return rela;
	}
	else if (firstBI->High <= secondBI->High && firstBI->Low >= secondBI->Low) {
		rela.isBaoHan = true;
		rela.BHType = BaoHanRela::HouDa;
		return rela;
	}
	else {
		rela.isBaoHan = false;
		rela.BHType = BaoHanRela::No;
		return rela;
	}
}


__declspec(dllexport) vector<BI*> ProcessBaoHanTZXL(vector<BI*> BIVector_Clean, BI* BiIn, BaoHanRela baoHanRela, TZXLFXing::TZXLFXingType Needs)
{
	BI* Bi = new BI;
	if (Needs == TZXLFXing::DING) {
		if (baoHanRela.BHType == BaoHanRela::QianDa) {
			Bi->High = BIVector_Clean[BIVector_Clean.size() - 1]->High;
			Bi->Low = BiIn->Low;
			Bi->Start = BIVector_Clean[BIVector_Clean.size() - 1]->Start;
			Bi->End = BiIn->End;
		}
		else {
			Bi->High = BiIn->High;
			Bi->Low = BIVector_Clean[BIVector_Clean.size() - 1]->Low;
			Bi->Start = BiIn->Start;
			Bi->End = BIVector_Clean[BIVector_Clean.size() - 1]->End;
		}
	}
	else {
		if (baoHanRela.BHType == BaoHanRela::QianDa) {
			Bi->High = BiIn->High;
			Bi->Low = BIVector_Clean[BIVector_Clean.size() - 1]->Low;
			Bi->Start = BIVector_Clean[BIVector_Clean.size() - 1]->Start;
			Bi->End = BiIn->End;
		}
		else {
			Bi->High = BIVector_Clean[BIVector_Clean.size() - 1]->High;;
			Bi->Low = BiIn->Low;
			Bi->Start = BiIn->Start;
			Bi->End= BIVector_Clean[BIVector_Clean.size() - 1]->End;
		}
	}
	//包含完成，最后一个必然失效，但是包含结果还需继续和再前一个处理包含
	BIVector_Clean.pop_back();
	if (BIVector_Clean.size() == 0) {
		BIVector_Clean.push_back(Bi);
	}
	else {
		BaoHanRela rela = BaoHanTZXL(BIVector_Clean[BIVector_Clean.size() - 1], Bi);
		if (rela.isBaoHan == false) {
			BIVector_Clean.push_back(Bi);
		}
		else {
			BIVector_Clean = ProcessBaoHanTZXL(BIVector_Clean, Bi, rela, Needs);
		}
	}
	return BIVector_Clean;
}

__declspec(dllexport) vector<TZXLFXing*> Generate_TZXL_BH_FX_Vector(vector<BI*> ShangXiaVector, TZXLFXing::TZXLFXingType Needs)
{
	//Shang Needs DI, Xia Needs DING
	vector<BI*> BIVector_Clean;
	BIVector_Clean.push_back(ShangXiaVector[0]);
	for (int i = 1; i < ShangXiaVector.size(); i++) {
		int temp_lenght = BIVector_Clean.size();
		BaoHanRela baoHanRela = BaoHanTZXL(BIVector_Clean[temp_lenght - 1], ShangXiaVector[i]);
		if (baoHanRela.isBaoHan) {
			BIVector_Clean = ProcessBaoHanTZXL(BIVector_Clean, ShangXiaVector[i], baoHanRela, Needs);
		}
		else {
			BIVector_Clean.push_back(ShangXiaVector[i]);
		}
	}
	if(Needs==TZXLFXing::DING)
		return  Generate_TZXL_Xia_FX_Vector(BIVector_Clean);
	else
		return  Generate_TZXL_Shang_FX_Vector(BIVector_Clean);
}

/*
为了画出第一段，给第一笔开始处加一个TZXLFX
前提是第一个特征序列极点前至少有两笔
*/
__declspec(dllexport) vector<TZXLFXing*> Add_First_TZXL_FX(vector<TZXLFXing*> Ordered_TZXL_Vector, vector<BI*> BIVector)
{
	if (Ordered_TZXL_Vector.size() == 0) return Ordered_TZXL_Vector;
	if (BIVector.size() <= 3) return Ordered_TZXL_Vector;
	TZXLFXing* tzxlfx = Ordered_TZXL_Vector[0];
	int tz_start = tzxlfx->Second->Start;
	if (BIVector[2]->Start >= tz_start) return Ordered_TZXL_Vector;
	TZXLFXing* newTZXLFX = new TZXLFXing;
	if (tzxlfx->Type == TZXLFXing::DING) {
		//需要从前面找一个上的笔
		BI* Shang = BIVector[0]->BITpye == BI::SHANG ? BIVector[0] : BIVector[1];
		newTZXLFX->Second = Shang;
		newTZXLFX->Type = TZXLFXing::DI;
		newTZXLFX->Valid = false;
	}
	else {
		BI* Xia = BIVector[0]->BITpye == BI::XIA ? BIVector[0] : BIVector[1];
		newTZXLFX->Second = Xia;
		newTZXLFX->Type = TZXLFXing::DING;
		newTZXLFX->Valid = false;
	}
	Ordered_TZXL_Vector.insert(Ordered_TZXL_Vector.begin(), newTZXLFX);
	return Ordered_TZXL_Vector;
}

__declspec(dllexport) SearchDuan* FindFirstDuan(vector<BI*> BIVector)
{
	SearchDuan* duan = new SearchDuan;
	duan->Valid = false;
	for (int i = 0; i < BIVector.size() - 2; i++) {
		BI* first_bi = BIVector[i];
		BI* second_bi = BIVector[i + 2];
		if (first_bi->BITpye == BI::SHANG) {
			if (first_bi->Low < second_bi->Low && first_bi->High < second_bi->High) {
				duan->Valid = true;
				duan->Start = first_bi;
				duan->End = second_bi;
				duan->Type = SearchDuan::Shang;
				return duan;
			}
		}
		else {
			if (first_bi->Low > second_bi->Low&& first_bi->High > second_bi->High) {
				duan->Valid = true;
				duan->Start = first_bi;
				duan->End = second_bi;
				duan->Type = SearchDuan::Xia;
				return duan;
			}
		}
	}
	return duan;
}