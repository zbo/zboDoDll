#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

__declspec(dllexport) vector<FXing*> AdjustGapInFX(vector<FXing*> FXVector)
{
	int sizeofFXVector = FXVector.size();
	for (int i = 0; i < sizeofFXVector; i++) {
		if (FXVector[i]->contain_gap())
		{
			if (FXVector[i]->FxType == FXing::Ding)
			{
				FXVector[i]->First->Low = FXVector[i]->Second->Low-0.01;
				FXVector[i]->Third->Low = FXVector[i]->Second->Low-0.01;
			}
			else {
				FXVector[i]->First->High = FXVector[i]->Second->High+0.01;
				FXVector[i]->Third->High = FXVector[i]->Second->High+0.01;
			}
		}
	}
	return FXVector;
}