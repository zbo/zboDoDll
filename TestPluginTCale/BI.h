#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"


FXSearchResult Finx_Next_FX_Index_FromAll(int firstFX_Index, vector<FXing*> FXVector);
FXSearchResult FindNextFromFloat(int secondFX_Index, vector<FXing*> FXVector);
int FindNextFromSolid(int firstFX_Index, vector<FXing*>& FXVector);
BI* FindFirstBI(vector<FXing*> FXVector);
FXSearchResult Find_First_FX_FromALL(vector<FXing*> FXVector);
FXSearchResult Finx_Next_FX_FromAll(FXSearchResult result1, vector<FXing*> FXVector);
vector<int> Generate_Final_Index(FXSearchResult& result1, vector<FXing*> FXVector);
vector<FXing*> Generate_Final_FX(vector<int> final_index, vector<FXing*> FXVector);
