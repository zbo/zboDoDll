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

vector<FXing*> LoadFXPH(vector<KXian*>& KXianVector)
{
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result1 = Find_First_FX_FromALL(FXVector);
	vector<int> final = Generate_Final_Index(result1, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(final, FXVector);
	return Final_FXVector;
}



TEST(ALL_POHUAI, 603388_BH_DUAN) {
	TestDataBag* bag = LoadData("..\\testdata\\603388.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFXPH(KXianVector);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	SearchDuan* FirstDuan = FindFirstDuan(BIVector);
	vector<SearchDuan*> DuanVector;
	DuanVector.push_back(FirstDuan);
}
