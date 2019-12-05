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
	FXSearchResult result = FindNextFromFloat(secondFX_Index, FXVector);
	return result;
}