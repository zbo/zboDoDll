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





TEST(ALL_UNIT, Test_Can_Find_First_From_Simple3) {
	TestDataBag* bag = LoadData("..\\testdata\\simple3.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 15);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	int index= result.FirstFX_Index;
	FXing* firstFX = FXVector[index];
	EXPECT_FLOAT_EQ(firstFX->Second->Low, 1.0);
	EXPECT_FLOAT_EQ(firstFX->FxType, FXing::Di);
	EXPECT_EQ(firstFX->First->i, 6);
	EXPECT_EQ(firstFX->Second->i, 7);
	EXPECT_EQ(firstFX->Third->i, 8);
}
TEST(ALL_UNIT, Test_Can_Find_First_From_Simple2) {
	TestDataBag* bag = LoadData("..\\testdata\\simple2.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 14);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	FXing* firstFX = FXVector[result.SecondFX_Index];
	EXPECT_FLOAT_EQ(firstFX->Second->High, 5.3);
	EXPECT_FLOAT_EQ(firstFX->FxType, FXing::Ding);
	EXPECT_EQ(firstFX->First->i, 1);
	EXPECT_EQ(firstFX->Second->i, 2);
	EXPECT_EQ(firstFX->Third->i, 5);
}
TEST(ALL_UNIT, Test_Can_Find_BH_From_SimpleDing) {
	TestDataBag* bag = LoadData("..\\testdata\\simple1.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 8);
	vector<FXing*> FXVector = Find_Ding_FX_BH(KXianVector);
	EXPECT_FLOAT_EQ(FXVector[0]->Second->High,5.3);
	EXPECT_FLOAT_EQ(FXVector[0]->Second->Low, 2.3);
	EXPECT_FLOAT_EQ(FXVector[0]->First->High, 2.2);
	EXPECT_FLOAT_EQ(FXVector[0]->First->Low, 1.2);
	EXPECT_FLOAT_EQ(FXVector[0]->Third->High, 2.2);
	EXPECT_FLOAT_EQ(FXVector[0]->Third->Low, 1.2);
}

TEST(ALL_UNIT, Test_CanFindMaxMinFromKXianVector) {
	TestDataBag* bag = LoadData("..\\testdata\\simple1.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(),8);
	vector<KXian*> KXianMaxMinVector = FindMaxMinFromKXianVector(KXianVector);
	EXPECT_EQ(KXianMaxMinVector.size(), 2);
	EXPECT_FLOAT_EQ(KXianMaxMinVector[0]->High, 5.3);
	EXPECT_FLOAT_EQ(KXianMaxMinVector[1]->Low, 1);
}

TEST(E2E, DAY420) {
	fstream file;
	file.open("..\\testdata\\debug2.txt", ios::in);
	vector<float> pfINa_high;
	vector<float> pfINb_low;
	char buf[1024];
	while (file.getline(buf, sizeof(buf))) {
	std:string str(buf);
		vector<string> strarr = mySplit(str, " ");
		float high = atof(strarr[0].c_str());
		pfINa_high.push_back(high);
		float low = atof(strarr[1].c_str());
		pfINb_low.push_back(low);
		cout << str << endl;
	}
	file.close();
	int DataLength = pfINa_high.size();
	float* out = new float[DataLength];
	float* pfINa = new float[DataLength];
	float* pfINb = new float[DataLength];
	float* pfINc = new float[DataLength];
	memcpy(pfINa, &pfINa_high[0], pfINa_high.size() * sizeof(float));
	memcpy(pfINb, &pfINb_low[0], pfINb_low.size() * sizeof(float));
	TestPlugin2(DataLength, out, pfINa, pfINb, pfINc);
}

TEST(ALL_UNIT, 002961_Can_Find_FirstFX_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002961.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 59);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	int first_index = result.FirstFX_Index;
	FXSearchResult next_result1 = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	FXSearchResult next_result2 = Finx_Next_FX_Index_FromAll(next_result1.FirstFX_Index, FXVector);
	FXSearchResult next_result3 = Finx_Next_FX_Index_FromAll(next_result2.FirstFX_Index, FXVector);
	EXPECT_EQ(FXVector.size(), 15);
	EXPECT_EQ(FXVector[first_index]->FxType, FXing::Di);
	EXPECT_FLOAT_EQ(FXVector[first_index]->Second->Low, 27.33);
	EXPECT_EQ(next_result1.FirstFX->FxType, FXing::Ding);
	EXPECT_FLOAT_EQ(next_result1.FirstFX->Second->High, 36.77);

}

TEST(ALL_UNIT, 002958_Can_Find_FirstFX_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002958.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 171);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	int first_index = result.FirstFX_Index;
	FXSearchResult next_result1 = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	FXSearchResult next_result2 = Finx_Next_FX_Index_FromAll(next_result1.FirstFX_Index, FXVector);
	FXSearchResult next_result3 = Finx_Next_FX_Index_FromAll(next_result2.FirstFX_Index, FXVector);
	EXPECT_EQ(FXVector.size(), 51);
	EXPECT_EQ(next_result1.FirstFX_Index, 25);
	EXPECT_EQ(next_result1.SecondFX_Index, 30);
	EXPECT_EQ(next_result2.FirstFX_Index, 34);
	EXPECT_EQ(next_result2.SecondFX_Index, 37);
	EXPECT_EQ(next_result3.FirstFX_Index, 39);
	EXPECT_EQ(next_result3.SecondFX_Index, 42);
}

TEST(ALL_UNIT, 002957_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002957.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 87);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	int first_index =result.FirstFX_Index;
	FXSearchResult next_result = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	EXPECT_EQ(FXVector.size(), 33);
	EXPECT_EQ(FXVector[first_index]->FxType, FXing::Di);
	EXPECT_FLOAT_EQ(FXVector[first_index]->Second->Low, 32.56);
	EXPECT_EQ(next_result.FirstFX->FxType, FXing::Ding);
	EXPECT_FLOAT_EQ(next_result.FirstFX->Second->High, 45.59);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(first_index, FXVector);
	EXPECT_EQ(Final_FXVector.size(),4);
}

TEST(ALL_UNIT, 002968_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002968.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	int first_index = result.SecondFX_Index;
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(first_index, FXVector);
}

TEST(ALL_UNIT, 999999_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\999999.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.SecondFX_Index, FXVector);
	EXPECT_EQ(FXVector.size(), 9);
	EXPECT_FLOAT_EQ(FXVector[6]->Second->High, 2901.36);
	EXPECT_FLOAT_EQ(FXVector[6]->Second->Low, 2879.69);
}

TEST(ALL_UNIT, 600000_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\600000.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.FirstFX_Index, FXVector);
	/*ISSUE Between index 2 and 3*/
	FXSearchResult next_result1 = Finx_Next_FX_Index_FromAll(result.SecondFX_Index, FXVector);
	int second_index = next_result1.FirstFX_Index;
	FXSearchResult next_result2 = Finx_Next_FX_Index_FromAll(second_index, FXVector);
	int third_index = next_result2.FirstFX_Index;
	FXSearchResult next_result3 = Finx_Next_FX_Index_FromAll(third_index, FXVector);
	//EXPECT_EQ(FXVector.size(), 9);
}


TEST(ALL_UNIT, 600000_New_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\600000.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result1= Find_First_FX_FromALL(FXVector);
	EXPECT_EQ(result1.FirstFX->FxType, FXing::Ding);
	EXPECT_EQ(result1.SecondFX->FxType, FXing::Di);
	EXPECT_EQ(result1.FirstFX_Index, 2);
	EXPECT_EQ(result1.SecondFX_Index, 3);
	vector<int> final =Generate_Final_Index(result1, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(final, FXVector);

	EXPECT_EQ(1, 1);
	/*FXSearchResult result2 = Finx_Next_FX_FromAll(result1,FXVector);
	FXSearchResult result3 = Finx_Next_FX_FromAll(result2, FXVector);
	FXSearchResult result4 = Finx_Next_FX_FromAll(result3, FXVector);*/
}

TEST(ALL_UNIT, 002953_New_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002953.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result1 = Find_First_FX_FromALL(FXVector);
	EXPECT_EQ(KXianVector.size(), 144);
	vector<int> final = Generate_Final_Index(result1, FXVector);
	EXPECT_EQ(final.size(), 9);
	vector<FXing*> Final_FXVector = Generate_Final_FX(final, FXVector);
	EXPECT_EQ(1, 1);

}