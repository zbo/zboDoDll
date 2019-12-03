#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class TestDataBag {
public:
	int DataLength;
	float* out;
	float* pfINa;
	float* pfINb;
	float* pfINc;

};

TestDataBag* LoadData(std::string file_path) {
	TestDataBag* bag = new TestDataBag;
	fstream file;
	file.open(file_path, ios::in);
	vector<float> pfINa_high;
	vector<float> pfINb_low;
	vector<float> pfINc_date;
	char buf[1024];
	while (file.getline(buf, sizeof(buf))) {
	std:string str(buf);
		vector<string> strarr = mySplit(str, " ");
		float high = atof(strarr[0].c_str());
		pfINa_high.push_back(high);
		float low = atof(strarr[1].c_str());
		pfINb_low.push_back(low);
		float date1 = atof(strarr[2].c_str());
		pfINc_date.push_back(date1);
	}
	file.close();
	int DataLength = pfINa_high.size();
	float* out = new float[DataLength];
	float* pfINa = new float[DataLength];
	float* pfINb = new float[DataLength];
	float* pfINc = new float[DataLength];
	memcpy(pfINa, &pfINa_high[0], pfINa_high.size() * sizeof(float));
	memcpy(pfINb, &pfINb_low[0], pfINb_low.size() * sizeof(float));
	memcpy(pfINc, &pfINc_date[0], pfINc_date.size() * sizeof(float));
	bag->DataLength = DataLength;
	bag->out = out;
	bag->pfINa = pfINa;
	bag->pfINb = pfINb;
	bag->pfINc = pfINc;
	return bag;
}

TEST(ALL_UNIT, Test_Can_Find_First_From_Simple3) {
	TestDataBag* bag = LoadData("..\\testdata\\simple3.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 15);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	int index= Find_First_FX_Index_FromALL(FXVector);
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
	FXing* firstFX = FXVector[Find_First_FX_Index_FromALL(FXVector)];
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

TEST(ALL_UNIT, 002957_Can_Find_FirstFX_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002957.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 87);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	int first_index = Find_First_FX_Index_FromALL(FXVector);
	FXSearchResult next_result = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	EXPECT_EQ(FXVector.size(), 33);
	EXPECT_FLOAT_EQ(FXVector[first_index]->Second->High, 45.25);
	EXPECT_FLOAT_EQ(next_result.SecondFX->Second->Low,32.56);
}

TEST(ALL_UNIT, 002957_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\002957.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 87);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	int first_index = Find_First_FX_Index_FromALL(FXVector);
	FXSearchResult next_result = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	EXPECT_EQ(FXVector.size(), 33);
	EXPECT_FLOAT_EQ(FXVector[first_index]->Second->High, 45.25);
	EXPECT_FLOAT_EQ(next_result.SecondFX->Second->Low, 32.56);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(first_index, FXVector);
	EXPECT_EQ(Final_FXVector.size(),5);
}