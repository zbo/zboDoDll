#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include "../TestPluginTCale/BI.h"
#include "../TestPluginTCale/FX.h"
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


BI* CreateBI(FXing* FX1, FXing* FX2)
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

vector<BI*> GenerateBIVector(vector<FXing*> Final_FXVector)
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

TEST(ALL_DUAN, 002949_Can_Find_DUAN) {
	TestDataBag* bag = LoadData("..\\testdata\\002949.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	EXPECT_EQ(KXianVector.size(), 192);
	EXPECT_EQ(Final_FXVector.size(), 11);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	EXPECT_EQ(10, BIVector.size());
	vector<BI*> TZXL_Shang;
	vector<BI*> TZXL_Xia;
}