#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include "../TestPluginTCale/BI.h"
#include "../TestPluginTCale/FX.h"
#include "../TestPluginTCale/DUAN.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

vector<FXing*> LoadFX(vector<KXian*>& KXianVector)
{
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result1 = Find_First_FX_FromALL(FXVector);
	vector<int> final = Generate_Final_Index(result1, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(final, FXVector);
	return Final_FXVector;
}

TEST(ALL_DUAN, 002949_Can_Find_DUAN) {
	TestDataBag* bag = LoadData("..\\testdata\\002949.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	EXPECT_EQ(KXianVector.size(), 192);
	EXPECT_EQ(Final_FXVector.size(), 11);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	EXPECT_EQ(10, BIVector.size());
	vector<BI*> TZXL_Shang = GenerateTZXL_Shang(BIVector);
	vector<TZXLFXing*> TZXL_Shang_Vector = Generate_TZXL_Shang_FX_Vector(TZXL_Shang);
	vector<BI*> TZXL_Xia = GenerateTZXL_Xia(BIVector);
	vector<TZXLFXing*> TZXL_Xia_Vector = Generate_TZXL_Xia_FX_Vector(TZXL_Xia);

	EXPECT_EQ(TZXL_Shang_Vector.size(), 1);
	EXPECT_EQ(TZXL_Xia_Vector.size(), 1);
	vector<TZXLFXing*> Ordered_TZXL_Vector = Ordered_TZXL_FX(TZXL_Shang_Vector, TZXL_Xia_Vector);
	EXPECT_EQ(Ordered_TZXL_Vector.size(), 2);
	EXPECT_EQ(Ordered_TZXL_Vector[0]->Type, TZXLFXing::DI);
	EXPECT_EQ(Ordered_TZXL_Vector[1]->Type, TZXLFXing::DING);
	EXPECT_EQ(Ordered_TZXL_Vector.size(), 2);
	EXPECT_FLOAT_EQ(Ordered_TZXL_Vector[0]->Second->Low, 18.8);
	EXPECT_FLOAT_EQ(Ordered_TZXL_Vector[1]->Second->High, 34.6);
}