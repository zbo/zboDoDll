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


TEST(ALL_DUAN, 002947_Can_Find_DUAN) {
	TestDataBag* bag = LoadData("..\\testdata\\002947.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	EXPECT_EQ(10, BIVector.size());
	vector<BI*> TZXL_Shang = GenerateTZXL_Shang(BIVector);
	vector<TZXLFXing*> TZXL_Shang_Vector = Generate_TZXL_Shang_FX_Vector(TZXL_Shang);
	vector<TZXLFXing*> TZXL_BH_Shang_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Shang, TZXLFXing::DI);

	vector<BI*> TZXL_Xia = GenerateTZXL_Xia(BIVector);
	vector<TZXLFXing*> TZXL_Xia_Vector = Generate_TZXL_Xia_FX_Vector(TZXL_Xia);
	vector<TZXLFXing*> TZXL_BH_Xia_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Xia, TZXLFXing::DING);

	vector<TZXLFXing*> Vector1 = Ordered_TZXL_FX(TZXL_Shang_Vector, TZXL_BH_Shang_Vector);
	vector<TZXLFXing*> Vector2 = Ordered_TZXL_FX(TZXL_Xia_Vector, TZXL_BH_Xia_Vector);
	vector<TZXLFXing*> Ordered_TZXL_Vector = Ordered_TZXL_FX(Vector1,Vector2);
}

TEST(ALL_DUAN, 603386_Can_Find_DUAN) {

	TestDataBag* bag = LoadData("..\\testdata\\603386.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	EXPECT_EQ(36, BIVector.size());
	vector<BI*> TZXL_Shang = GenerateTZXL_Shang(BIVector);
	vector<TZXLFXing*> TZXL_Shang_Vector = Generate_TZXL_Shang_FX_Vector(TZXL_Shang);
	vector<TZXLFXing*> TZXL_BH_Shang_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Shang, TZXLFXing::DI);

	vector<BI*> TZXL_Xia = GenerateTZXL_Xia(BIVector);
	vector<TZXLFXing*> TZXL_Xia_Vector = Generate_TZXL_Xia_FX_Vector(TZXL_Xia);
	vector<TZXLFXing*> TZXL_BH_Xia_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Xia, TZXLFXing::DING);

	vector<TZXLFXing*> Vector1 = Ordered_TZXL_FX(TZXL_Shang_Vector, TZXL_BH_Shang_Vector);
	vector<TZXLFXing*> Vector2 = Ordered_TZXL_FX(TZXL_Xia_Vector, TZXL_BH_Xia_Vector);
	vector<TZXLFXing*> Ordered_TZXL_Vector = Ordered_TZXL_FX(Vector1, Vector2);
	Ordered_TZXL_Vector = Add_First_TZXL_FX(Ordered_TZXL_Vector,BIVector);
}


vector<TZXLFXing*> RemoveDuplicated(vector<TZXLFXing*> TZXL_BH_Vector, vector<TZXLFXing*> TZXL_Vector)
{
	vector<TZXLFXing*> removed;
	for (int i = 0; i < TZXL_BH_Vector.size(); i++) {
		bool exist = false;
		for (int j = 0; j < TZXL_Vector.size(); j++) {
			if (TZXL_BH_Vector[i]->First->Start == TZXL_Vector[j]->First->Start &&
				TZXL_BH_Vector[i]->First->End == TZXL_Vector[j]->First->End &&
				TZXL_BH_Vector[i]->Third->Start == TZXL_Vector[j]->Third->Start &&
				TZXL_BH_Vector[i]->Third->End == TZXL_Vector[j]->Third->End) {
				exist = true;
				break;
			}
		}
		if (!exist) {
			removed.push_back(TZXL_BH_Vector[i]);
		}
	}
	return removed;
}

TEST(ALL_DUAN, 603880_Can_Find_DUAN) {

	TestDataBag* bag = LoadData("..\\testdata\\603880.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> Final_FXVector = LoadFX(KXianVector);
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	EXPECT_EQ(29, BIVector.size());
	vector<BI*> TZXL_Shang = GenerateTZXL_Shang(BIVector);
	vector<TZXLFXing*> TZXL_Shang_Vector = Generate_TZXL_Shang_FX_Vector(TZXL_Shang);
	vector<TZXLFXing*> TZXL_BH_Shang_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Shang, TZXLFXing::DI);
	TZXL_BH_Shang_Vector = RemoveDuplicated(TZXL_BH_Shang_Vector,TZXL_Shang_Vector);

	vector<BI*> TZXL_Xia = GenerateTZXL_Xia(BIVector);
	vector<TZXLFXing*> TZXL_Xia_Vector = Generate_TZXL_Xia_FX_Vector(TZXL_Xia);
	vector<TZXLFXing*> TZXL_BH_Xia_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Xia, TZXLFXing::DING);
	TZXL_BH_Xia_Vector = RemoveDuplicated(TZXL_BH_Xia_Vector,TZXL_Xia_Vector);


	vector<TZXLFXing*> Vector1 = Ordered_TZXL_FX(TZXL_Shang_Vector, TZXL_BH_Shang_Vector);
	vector<TZXLFXing*> Vector2 = Ordered_TZXL_FX(TZXL_Xia_Vector, TZXL_BH_Xia_Vector);
	vector<TZXLFXing*> Ordered_TZXL_Vector = Ordered_TZXL_FX(Vector1, Vector2);
	Ordered_TZXL_Vector = Add_First_TZXL_FX(Ordered_TZXL_Vector, BIVector);
	EXPECT_EQ(Ordered_TZXL_Vector.size(), 6);
}