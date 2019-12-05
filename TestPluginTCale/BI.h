#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"


FXSearchResult Finx_Next_FX_Index_FromAll(int firstFX_Index, vector<FXing*> FXVector);
FXSearchResult FindNextFromFloat(int secondFX_Index, vector<FXing*> FXVector);
int FindNextFromSolid(int firstFX_Index, vector<FXing*>& FXVector);
