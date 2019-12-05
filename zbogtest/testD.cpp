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

vector<BI*> GenerateBIVector(vector<FXing*> Final_FXVector)
{
	throw std::logic_error("The method or operation is not implemented.");
}

TEST(ALL_DUAN, 002949_Can_Find_DUAN) {
	TestDataBag* bag = LoadData("..\\testdata\\002949.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	EXPECT_EQ(KXianVector.size(), 192);
	EXPECT_EQ(Final_FXVector.size(), 11);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);

}