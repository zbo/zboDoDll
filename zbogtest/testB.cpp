#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include "../TestPluginTCale/BI.h"
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
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
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
	int first_index = result.SecondFX_Index;
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(first_index, FXVector);
}

TEST(ALL_UNIT, 999999_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\999999.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.SecondFX_Index, FXVector);
	EXPECT_EQ(FXVector.size(), 9);
	EXPECT_FLOAT_EQ(FXVector[6]->Second->High, 2901.36);
	EXPECT_FLOAT_EQ(FXVector[6]->Second->Low, 2879.69);
}

TEST(ALL_UNIT, 600000_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\600000.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.FirstFX_Index, FXVector);
	/*ISSUE Between index 2 and 3*/
	FXSearchResult next_result1 = Finx_Next_FX_Index_FromAll(result.SecondFX_Index, FXVector);
	int second_index = next_result1.FirstFX_Index;
	FXSearchResult next_result2 = Finx_Next_FX_Index_FromAll(second_index, FXVector);
	int third_index = next_result2.FirstFX_Index;
	FXSearchResult next_result3 = Finx_Next_FX_Index_FromAll(third_index, FXVector);
	//EXPECT_EQ(FXVector.size(), 9);
}
bool DingDiDuLi(FXing* FxDing, FXing* FxDi)
{
	bool dingDuLi = FxDing->Second->Low > FxDi->First->High&&
		FxDing->Second->Low > FxDi->Second->High&&
		FxDing->Second->Low > FxDi->Second->High;

	bool diDuLi = FxDi->Second->High < FxDing->First->Low&&
		FxDi->Second->High < FxDing->Second->Low &&
		FxDi->Second->High < FxDing->Third->Low;
	return dingDuLi && diDuLi;
}

FXSearchResult Finx_Next_FX_FromAll(FXSearchResult result1, vector<FXing*> FXVector)
{
	FXSearchResult result2;
	result2.FirstFX_Confirmed = true;
	int first_index = result1.FirstFX_Index;
	int second_index = result1.SecondFX_Index;
	for (int i = second_index + 1; i < FXVector.size() - 1; i++) {
		/*¶¥½Ó¶¥*/
		if (result1.SecondFX->FxType == FXing::Ding &&
			FXVector[i]->FxType == FXing::Ding) {
			if (FXVector[i]->Second->High > result1.SecondFX->Second->High) {
				result1.SecondFX = FXVector[i];
				result1.SecondFX_Index = i;
				return result1;
			}}
		/*µ×½Óµ×*/
		if (result1.SecondFX->FxType == FXing::Di &&
			FXVector[i]->FxType==FXing::Di) {
			if (FXVector[i]->Second->Low < result1.SecondFX->Second->Low) {
				result1.SecondFX = FXVector[i];
				result1.SecondFX_Index = i;
				return result1;
			}}
		if (result1.SecondFX->FxType == FXing::Ding &&
			FXVector[i]->FxType == FXing::Di) {
			if (DingDiDuLi(result1.SecondFX,FXVector[i])) {
				result2.FirstFX = result1.SecondFX;
				result2.FirstFX_Index = result1.SecondFX_Index;
				result2.SecondFX = FXVector[i];
				result2.SecondFX_Index = i;
				return result2;
			}
		}
		if (result1.SecondFX->FxType == FXing::Di &&
			FXVector[i]->FxType == FXing::Ding) {
			if (DingDiDuLi(FXVector[i],result1.SecondFX)) {
				result2.FirstFX = result1.SecondFX;
				result2.FirstFX_Index = result1.SecondFX_Index;
				result2.SecondFX = FXVector[i];
				result2.SecondFX_Index = i;
				return result2;
			}
		}
	}
	result2.FirstFX_Confirmed = false;
	return result2;
}


TEST(ALL_UNIT, 600000_New_Can_Find_ALL_From_FXVector) {
	TestDataBag* bag = LoadData("..\\testdata\\600000.txt");
	vector<KXian*> KXianVector = GenerateKXianVector(bag->DataLength, bag->out, bag->pfINa, bag->pfINb, bag->pfINc);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result1= Find_First_FX_Index_FromALL(FXVector);
	EXPECT_EQ(result1.FirstFX->FxType, FXing::Ding);
	EXPECT_EQ(result1.SecondFX->FxType, FXing::Di);
	EXPECT_EQ(result1.FirstFX_Index, 2);
	EXPECT_EQ(result1.SecondFX_Index, 3);
	vector<int> final_FX_Index;
	final_FX_Index.push_back(result1.FirstFX_Index);
	final_FX_Index.push_back(result1.SecondFX_Index);
	while (result1.FirstFX_Confirmed) {
		FXSearchResult result2 = Finx_Next_FX_FromAll(result1, FXVector);
		result1 = result2;
		if (result1.FirstFX_Confirmed == false) { break; }
		int kept_second = final_FX_Index[final_FX_Index.size() - 1];
		int kept_first = final_FX_Index[final_FX_Index.size() - 2];
		if (kept_first == result2.FirstFX_Index) {
			final_FX_Index[final_FX_Index.size() - 1] = result2.SecondFX_Index;
		}
		else if (kept_second == result2.FirstFX_Index) {
			final_FX_Index.push_back(result2.SecondFX_Index);
		}
		else { 
			throw std::logic_error("The method or operation is not implemented."); 
		}
	}
	EXPECT_EQ(1, 1);
	/*FXSearchResult result2 = Finx_Next_FX_FromAll(result1,FXVector);

	FXSearchResult result3 = Finx_Next_FX_FromAll(result2, FXVector);

	FXSearchResult result4 = Finx_Next_FX_FromAll(result3, FXVector);*/

}