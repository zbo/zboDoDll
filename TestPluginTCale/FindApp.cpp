#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

using namespace std;


bool NotExist(vector<FXing *> Clean_FXing_Vector, FXing* FXingMin)
{	int Vectorlength = Clean_FXing_Vector.size();
if(Vectorlength ==0){
	return true;
}
if(FXingMin->Second->i==Clean_FXing_Vector[Vectorlength-1]->Second->i){
	return false;
}
return true;
}

KXian* Find_DD_KX(FXing* FX, vector<KXian *> allKXianVector)
{
	int first_i = FX->First->i;
	int third_i = FX->Third->i;
	int i=first_i+1;
	KXian* kx = allKXianVector[i];
	while(i<third_i){
		i++;
		if(allKXianVector[i]->Low<kx->Low)
			kx=allKXianVector[i];
	}
	return kx;	
}

KXian* Find_GD_KX(FXing* FX, vector<KXian *> allKXianVector)
{
	int first_i = FX->First->i;
	int third_i = FX->Third->i;
	int i=first_i+1;
	KXian* kx = allKXianVector[i];
	while(i<third_i){
		i++;
		if(allKXianVector[i]->High>kx->High)
			kx=allKXianVector[i];
	}
	return kx;
}

int FindMiniumFXingIndex(vector<FXing *> FXVector)
{
	int min_index = 0;
	for(int i=0;i<FXVector.size();i++){
		if(FXVector[min_index]->Second->i>FXVector[i]->Second->i)
			min_index = i;
	}
	return min_index;
}

vector<FXing *> Clean_All_FX(vector<FXing *> FXVector)
{
	vector<FXing*> Clean_FXing_Vector;
	while(FXVector.size()>0)
	{
		int min_index =FindMiniumFXingIndex(FXVector);
		if(NotExist(Clean_FXing_Vector,FXVector[min_index])){
			Clean_FXing_Vector.push_back(FXVector[min_index]);
		}
		FXVector.erase(std::begin(FXVector)+min_index);
	}
	return Clean_FXing_Vector;
}

vector<FXing *> Adjust_All_FX(vector<FXing *> FXVector_Clean, vector<KXian *> allKXianVector)
{
	vector<FXing*> Adjust_FXing_Vector;
	for(int i=0;i<FXVector_Clean.size();i++){
		FXing* FX=FXVector_Clean[i];
		if(FX->Third->i>FX->First->i+2){
			 if(FX->FxType==FXing::Ding){
				 KXian* GDKXian=Find_GD_KX(FX,allKXianVector); 
				 FX->Second=GDKXian;
			 }else{
				 KXian* DDKXian=Find_DD_KX(FX,allKXianVector);
				 FX->Second=DDKXian;
			 }
		}
		Adjust_FXing_Vector.push_back(FX);
	}
	return Adjust_FXing_Vector;
}
__declspec(dllexport) vector<FXing*> Find_Final_FXVector(vector<FXing*> FXVector)
{
	vector<FXing*> retBI;
	return retBI;
}

int FindNextFromSolid(int firstFX_Index, vector<FXing*>& FXVector)
{
	int secondFX_Index = firstFX_Index;
	FXing* solidFirst = FXVector[firstFX_Index];
	for (int i = firstFX_Index; i < FXVector.size(); i++) {
		if (solidFirst->Third->i + 1 >= FXVector[i]->First->i)
			continue;
		else if (FXVector[i]->FxType == solidFirst->FxType)
			continue;
		else if (solidFirst->FxType == FXing::Ding &&
			solidFirst->Second->Low < FXVector[i]->Second->High)
			continue;
		else if (solidFirst->FxType == FXing::Di &&
			solidFirst->Second->High > FXVector[i]->Second->Low)
			continue;
		else {
			secondFX_Index = i;
			break;
		}
	}
	return secondFX_Index;
}

FXSearchResult FindNextFromFloat(int secondFX_Index, vector<FXing*> FXVector)
{
	int confirmed_Second_FX_Index = secondFX_Index;
	int float_Third_FX_Index = secondFX_Index;
	FXing* confirmed_Second_FX = FXVector[confirmed_Second_FX_Index];
	for (int i = secondFX_Index; i < FXVector.size(); i++) {
		if (FXVector[i]->FxType == confirmed_Second_FX->FxType)
		{
			if (confirmed_Second_FX->FxType == FXing::Ding &&
				confirmed_Second_FX->Second->High < FXVector[i]->Second->High) {
				confirmed_Second_FX_Index = i;
				confirmed_Second_FX = FXVector[i];	
			}
			else if (confirmed_Second_FX->FxType == FXing::Di &&
				confirmed_Second_FX->Second->Low > FXVector[i]->Second->Low) {
				confirmed_Second_FX_Index = i;
				confirmed_Second_FX = FXVector[i];
			}
		}
		else {
			if (confirmed_Second_FX->FxType == FXing::Ding)
				if (confirmed_Second_FX->Second->Low < FXVector[i]->Second->High ||
					confirmed_Second_FX->First->Low < FXVector[i]->Second->High ||
					confirmed_Second_FX->Third->Low < FXVector[i]->Second->High ||
					confirmed_Second_FX->Second->Low < FXVector[i]->First->High ||
					confirmed_Second_FX->Second->Low < FXVector[i]->Third->High)
					continue;
			if (confirmed_Second_FX->FxType == FXing::Di)
				if (confirmed_Second_FX->Second->High > FXVector[i]->Second->Low ||
					confirmed_Second_FX->First->High > FXVector[i]->Second->Low ||
					confirmed_Second_FX->Third->High > FXVector[i]->Second->Low ||
					confirmed_Second_FX->Second->High > FXVector[i]->First->Low ||
					confirmed_Second_FX->Second->High > FXVector[i]->Third->Low)
					continue;
			float_Third_FX_Index = i;
			break;
			}
		}
	FXSearchResult result;
	result.SecondFX = confirmed_Second_FX;
	result.ThirdFX = FXVector[float_Third_FX_Index];
	result.SecondFX_Confirmed = confirmed_Second_FX_Index == float_Third_FX_Index ? false : true;
	result.SecondFX_Index = confirmed_Second_FX_Index;
	result.ThirdFX_Index = float_Third_FX_Index;
	return result;
}

__declspec(dllexport) FXSearchResult Finx_Next_FX_Index_FromAll(int firstFX_Index, vector<FXing*> FXVector)
{
	int secondFX_Index = FindNextFromSolid(firstFX_Index, FXVector);
	FXSearchResult result=FindNextFromFloat(secondFX_Index, FXVector);
	return result;
}

int Find_First(FXing::DINGDI dingdi, vector<FXing*> FXVector) {
	for (int i = 0; i < FXVector.size(); i++) {
		if (FXVector[i]->FxType == dingdi)
		{
			return i;
		}
	}
}

__declspec(dllexport) int Find_First_FX_Index_FromALL(vector<FXing*> FXVector)
{
	FXing* tempFirstDing = nullptr;
	FXing* tempFirstDi = nullptr;
	int tempFirstDingIndex = 0;
	int tempFirstDiIndex = 0;
	bool firstDingValid = false;
	bool firstDiValid = false;
	
	tempFirstDingIndex = Find_First(FXing::Ding, FXVector);
	tempFirstDing = FXVector[tempFirstDingIndex];
	tempFirstDiIndex = Find_First(FXing::Di, FXVector);
	tempFirstDi = FXVector[tempFirstDiIndex];

	if (tempFirstDing!=nullptr) {
		for (int i = tempFirstDingIndex; i < FXVector.size(); i++) {
			firstDingValid = false;
			/*顶分型后接顶分型，保留高顶那个顶分型*/
			if (FXVector[i]->FxType == FXing::Ding) {
				if (tempFirstDing->Second->High < FXVector[i]->Second->High)
				{
					tempFirstDing = FXVector[i];
					tempFirstDingIndex = i;
				}
			}/*顶分型后接底分型*/
			else {/*中间有独立K线*/
				if (tempFirstDing->Third->i + 1 < FXVector[i]->First->i)
					if(tempFirstDing->Second->Low>FXVector[i]->Second->High&&
						tempFirstDing->Second->Low > FXVector[i]->First->High&&
						tempFirstDing->Second->Low > FXVector[i]->Third->High&&
						tempFirstDing->First->Low > FXVector[i]->Second->High&&
						tempFirstDing->Third->Low > FXVector[i]->Second->High)/*顶底K区间独立*/
					{
						firstDingValid = true; break;
					}
			}
		}
	}
	if(tempFirstDi!=nullptr) {
		for (int i = tempFirstDiIndex; i < FXVector.size(); i++) {
			firstDiValid = false;
			/*底分型后接底分型，保留高顶那个顶分型*/
			if (FXVector[i]->FxType == FXing::Di) {
				if (tempFirstDi->Second->Low > FXVector[i]->Second->Low) {
					tempFirstDi = FXVector[i];
					tempFirstDiIndex = i;
				}
			}/*底分型后接顶分型*/
			else {
				if (tempFirstDi->Third->i + 1 < FXVector[i]->First->i)
					if (tempFirstDi->Second->High < FXVector[i]->Second->Low &&
						tempFirstDi->Second->High<FXVector[i]->First->Low &&
						tempFirstDi->Second->High < FXVector[i]->Third->Low&&
						tempFirstDi->First->High < FXVector[i]->Second->Low &&
						tempFirstDi->Third->High < FXVector[i]->Second->Low
						)/*底顶K区间独立*/
					{
						firstDiValid = true; break;
					}
			}
		}
	}
	if (firstDingValid && firstDiValid) {
		return tempFirstDing->Second->i < tempFirstDi->Second->i ? tempFirstDingIndex : tempFirstDiIndex;
	}else if (!firstDingValid && firstDiValid) {
		return tempFirstDiIndex;
	}else if (firstDingValid && !firstDiValid) {
		return tempFirstDingIndex;
	}else {
		return 0;
	}
}

__declspec(dllexport) vector<FXing*> Find_ALL_FX_FromAll(int first_index, vector<FXing*> FXVector)
{
	vector<int> all_FX_Vector_Index;
	vector<FXing*> all_FX_Vector;
	all_FX_Vector_Index.push_back(first_index);
	FXSearchResult next_result = Finx_Next_FX_Index_FromAll(first_index, FXVector);
	all_FX_Vector_Index.push_back(next_result.SecondFX_Index);
	while (next_result.SecondFX_Confirmed) {
		int before_search = next_result.SecondFX_Index;
		next_result = Finx_Next_FX_Index_FromAll(next_result.SecondFX_Index, FXVector);
		int after_search = next_result.SecondFX_Index;
		if (before_search == after_search)break;
		all_FX_Vector_Index.push_back(next_result.SecondFX_Index);
	}
	for (int i = 0; i < all_FX_Vector_Index.size(); i++) {
		all_FX_Vector.push_back(FXVector[all_FX_Vector_Index[i]]);
	}
	return all_FX_Vector;
}

vector<FXing *> Find_Ding_FX_Without_BH(std::vector<KXian *> KXianVector)
{
	vector<FXing*> FXingVector;
	for(int i=1; i<KXianVector.size()-1;i++){
		bool left_high = KXianVector[i-1]->High<KXianVector[i]->High;
		bool left_low = KXianVector[i-1]->Low<KXianVector[i]->Low;
		bool right_high = KXianVector[i]->High>KXianVector[i+1]->High;
		bool right_low = KXianVector[i]->Low>KXianVector[i+1]->Low;
		if(left_high&&left_low&&right_high&&right_low){
			FXing* DingFXing = new FXing;
			DingFXing->First=KXianVector[i-1];
			DingFXing->Second=KXianVector[i];
			DingFXing->Third=KXianVector[i+1];
			DingFXing->FxType=FXing::Ding;
			FXingVector.push_back(DingFXing);
		}
	}
	return FXingVector;
}

vector<FXing *> Find_Di_FX_Without_BH(std::vector<KXian *> KXianVector)
{
	vector<FXing*> FXingVector;
	for(int i=1; i<KXianVector.size()-1;i++){
		bool left_high = KXianVector[i-1]->High>KXianVector[i]->High;
		bool left_low = KXianVector[i-1]->Low>KXianVector[i]->Low;
		bool right_high = KXianVector[i]->High<KXianVector[i+1]->High;
		bool right_low = KXianVector[i]->Low<KXianVector[i+1]->Low;
		if(left_high&&left_low&&right_high&&right_low){
			FXing* DingFXing = new FXing;
			DingFXing->First=KXianVector[i-1];
			DingFXing->Second=KXianVector[i];
			DingFXing->Third=KXianVector[i+1];
			DingFXing->FxType=FXing::Di;
			FXingVector.push_back(DingFXing);
		}
	}
	return FXingVector;
}

KXian* DeepCopy(KXian* KXianIn)
{
	KXian* KX = new KXian;
	KX->High=KXianIn->High;
	KX->i=KXianIn->i;
	KX->Low=KXianIn->Low;
	return KX;
}

__declspec(dllexport) vector<FXing *> Find_Ding_FX_BH(std::vector<KXian *> KXianVector)
{
	vector<KXian*> KXianVector_Clean;
	vector<FXing*> FXingVector;
	KXianVector_Clean.push_back(KXianVector[0]);
	for(int i=1; i<KXianVector.size()-1;i++){
		int temp_lenght=KXianVector_Clean.size();
		BaoHanRela baoHanRela = BaoHan(KXianVector_Clean[temp_lenght-1],KXianVector[i]);
		if(baoHanRela.isBaoHan){
			KXian* KX = new KXian;
			KX->i=i;
			if(baoHanRela.BHType==BaoHanRela::QianDa){
				KX->High=KXianVector_Clean[temp_lenght-1]->High;
				KX->Low=KXianVector[i]->Low;}
			else{
				KX->High=KXianVector[i]->High;
				KX->Low=KXianVector_Clean[temp_lenght-1]->Low;
			}
			KX->BHan=true;
			//delete baohan process finished K
			KXianVector_Clean.pop_back();
			KXianVector_Clean.push_back(KX);
		}
		else{
			KXianVector_Clean.push_back(KXianVector[i]);
		}
	}
	FXingVector = Find_Ding_FX_Without_BH(KXianVector_Clean);
	return FXingVector;
}

__declspec(dllexport) vector<FXing *> Find_Di_FX_BH(std::vector<KXian *> KXianVector)
{
	vector<KXian*> KXianVector_Clean;
	vector<FXing*> FXingVector;
	KXianVector_Clean.push_back(KXianVector[0]);
	for(int i=1; i<KXianVector.size()-1;i++){
		int temp_lenght=KXianVector_Clean.size();
		BaoHanRela baoHanRela = BaoHan(KXianVector_Clean[temp_lenght-1],KXianVector[i]);
		if(baoHanRela.isBaoHan){
			KXian* KX = new KXian;
			KX->i=i;
			if(baoHanRela.BHType==BaoHanRela::QianDa){
				KX->High=KXianVector[i]->High;
				KX->Low=KXianVector_Clean[temp_lenght-1]->Low;}
			else{
				KX->High=KXianVector_Clean[temp_lenght-1]->High;
				KX->Low=KXianVector[i]->Low;
			}
			KX->BHan=true;
			KXianVector_Clean.pop_back();
			KXianVector_Clean.push_back(KX);
		}
		else{
			KXianVector_Clean.push_back(KXianVector[i]);
		}
	}
	FXingVector = Find_Di_FX_Without_BH(KXianVector_Clean);
	return FXingVector;
}

__declspec(dllexport) vector<FXing *> Find_All_FX(std::vector<KXian *> KXianVector)
{
	vector<FXing*> FXingVector1 = Find_Ding_FX_Without_BH(KXianVector);
	vector<FXing*> FXingVector2 = Find_Di_FX_Without_BH(KXianVector);
	vector<FXing*> FXingVector3 = Find_Ding_FX_BH(KXianVector);
	vector<FXing*> FXingVector4 = Find_Di_FX_BH(KXianVector);

	vector<FXing*> FXingVectorAll;
	FXingVectorAll.insert(FXingVectorAll.end(),FXingVector1.begin(),FXingVector1.end());
	FXingVectorAll.insert(FXingVectorAll.end(),FXingVector2.begin(),FXingVector2.end());
	FXingVectorAll.insert(FXingVectorAll.end(),FXingVector3.begin(),FXingVector3.end());
	FXingVectorAll.insert(FXingVectorAll.end(),FXingVector4.begin(),FXingVector4.end());
	vector<FXing*> FXVector_Clean = Clean_All_FX(FXingVectorAll);
	vector<FXing*> FXVector_Adjust = Adjust_All_FX(FXVector_Clean, KXianVector);
	return FXVector_Adjust;
}
