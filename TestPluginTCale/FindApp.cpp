#include "stdafx.h"
#include "FindApp.h"
#include <algorithm>
using namespace std;


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

vector<FXing *> Find_Ding_FX_BH(std::vector<KXian *> KXianVector)
{
	vector<KXian*> KXianVector_Clean;
	vector<FXing*> FXingVector;
	KXianVector_Clean.push_back(DeepCopy(KXianVector[0]));
	for(int i=1; i<KXianVector.size()-1;i++){
		int temp_lenght=KXianVector_Clean.size();
		BaoHanRela baoHanRela = BaoHan(KXianVector_Clean[temp_lenght-1],KXianVector[i]);
		if(baoHanRela.isBaoHan){
			KXian* KX = new KXian;
			KX->i=i;
			if(baoHanRela.BaoHanType==-1){
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
			KXian* tempKX = DeepCopy(KXianVector[i]);
			KXianVector_Clean.push_back(tempKX);
		}
	}
	FXingVector = Find_Ding_FX_Without_BH(KXianVector_Clean);
	return FXingVector;
}

vector<FXing *> Find_Di_FX_BH(std::vector<KXian *> KXianVector)
{
	vector<KXian*> KXianVector_Clean;
	vector<FXing*> FXingVector;
	KXianVector_Clean.push_back(DeepCopy(KXianVector[0]));
	for(int i=1; i<KXianVector.size()-1;i++){
		int temp_lenght=KXianVector_Clean.size();
		BaoHanRela baoHanRela = BaoHan(KXianVector_Clean[temp_lenght-1],KXianVector[i]);
		if(baoHanRela.isBaoHan){
			KXian* KX = new KXian;
			KX->i=i;
			if(baoHanRela.BaoHanType==-1){
				KX->High=KXianVector[i]->High;
				KX->Low=KXianVector_Clean[i-temp_lenght]->Low;}
			else{
				KX->High=KXianVector_Clean[i-temp_lenght]->High;
				KX->Low=KXianVector[i]->Low;
			}
			KX->BHan=true;
			KXianVector_Clean.pop_back();
			KXianVector_Clean.push_back(KX);
		}
		else{
			KXian* tempKX = DeepCopy(KXianVector[i]);
			KXianVector_Clean.push_back(tempKX);
		}
	}
	FXingVector = Find_Di_FX_Without_BH(KXianVector_Clean);
	return FXingVector;
}

vector<FXing *> Find_All_FX(std::vector<KXian *> KXianVector)
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
	return FXingVectorAll;
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
