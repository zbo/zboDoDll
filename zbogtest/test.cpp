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
	bag->DataLength = DataLength;
	bag->out = out;
	bag->pfINa = pfINa;
	bag->pfINb = pfINb;
	bag->pfINc = pfINc;
	return bag;
}

TEST(E2E, DAY420_CanGenerateFinalFXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\debug2.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	EXPECT_EQ(KXianVector.size(), 420);
	vector<FXing*> FXVector = Find_Ding_FX_BH(KXianVector);

}
TEST(ALL_UNIT, Test_CanFindBHFromSimpleDing) {
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

TEST(Sample, HelloWorld) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
  float arr[10];
  int a = 1;
  FillinPOutDefault(arr, a);
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