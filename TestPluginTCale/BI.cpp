#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

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
			/*if (confirmed_Second_FX->FxType == FXing::Ding)
				if(confirmed_Second_FX->Second->High < FXVector[i]->Second->Low)
					continue;
			if (confirmed_Second_FX->FxType == FXing::Di)
				if (confirmed_Second_FX->Second->Low > FXVector[i]->Second->High)
					continue;*/
			float_Third_FX_Index = i;
			break;
		}
	}
	FXSearchResult result;
	result.FirstFX = confirmed_Second_FX;
	result.SecondFX = FXVector[float_Third_FX_Index];
	result.FirstFX_Confirmed = confirmed_Second_FX_Index == float_Third_FX_Index ? false : true;
	result.FirstFX_Index = confirmed_Second_FX_Index;
	result.SecondFX_Index = float_Third_FX_Index;
	return result;
}

__declspec(dllexport) FXSearchResult Finx_Next_FX_Index_FromAll(int firstFX_Index, vector<FXing*> FXVector)
{
	int secondFX_Index = FindNextFromSolid(firstFX_Index, FXVector);
	FXSearchResult result = FindNextFromFloat(secondFX_Index, FXVector);
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

__declspec(dllexport) FXSearchResult Find_First_FX_Index_FromALL(vector<FXing*> FXVector)
{
	FXSearchResult result;
	result.FirstFX_Index = 0;
	result.SecondFX_Index = 0;
	FXSearchResult tempResultDing;
	FXSearchResult tempResultDi;
	if (FXVector.size() == 0) return result;
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

	if (tempFirstDing != nullptr) {
		for (int i = tempFirstDingIndex+1; i < FXVector.size(); i++) {
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
					if (tempFirstDing->Second->Low > FXVector[i]->Second->High&&
						tempFirstDing->Second->Low > FXVector[i]->First->High&&
						tempFirstDing->Second->Low > FXVector[i]->Third->High&&
						tempFirstDing->First->Low > FXVector[i]->Second->High&&
						tempFirstDing->Third->Low > FXVector[i]->Second->High)/*顶底K区间独立*/
					{
						firstDingValid = true; 
						tempResultDing.SecondFX_Index = i;
						tempResultDing.SecondFX = FXVector[i];
						tempResultDing.FirstFX = tempFirstDing;
						tempResultDing.FirstFX_Index = tempFirstDingIndex;
						break;
					}
			}
		}
	}
	if (tempFirstDi != nullptr) {
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
						tempFirstDi->Second->High < FXVector[i]->First->Low &&
						tempFirstDi->Second->High < FXVector[i]->Third->Low &&
						tempFirstDi->First->High < FXVector[i]->Second->Low &&
						tempFirstDi->Third->High < FXVector[i]->Second->Low
						)/*底顶K区间独立*/
					{
						firstDiValid = true; 
						tempResultDi.SecondFX_Index = i;
						tempResultDi.SecondFX = FXVector[i];
						tempResultDi.FirstFX = tempFirstDi;
						tempResultDi.FirstFX_Index = tempFirstDiIndex;
						break;
					}
			}
		}
	}
	if (firstDingValid && firstDiValid) {
		if (tempFirstDing->Second->i < tempFirstDi->Second->i)
		{
			result = tempResultDing;
		}
		else {
			result = tempResultDi;
		}
	}
	else if (!firstDingValid && firstDiValid) {
		result = tempResultDi;
	}
	else if (firstDingValid && !firstDiValid) {
		result = tempResultDing;
	}
	else {
		result.FirstFX_Index = 0;
		result.SecondFX_Index = 0;
	}
	return result;
}

__declspec(dllexport) BI* FindFirstBI(vector<FXing*> FXVector)
{
	BI* first_bi = new BI;
	FXSearchResult result = Find_First_FX_Index_FromALL(FXVector);
	first_bi->FirstFX = result.FirstFX;
	first_bi->SecondFX = result.SecondFX;
	return first_bi;
}