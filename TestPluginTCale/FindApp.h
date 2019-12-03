#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

using namespace std;

vector<FXing *> Find_All_FX(std::vector<KXian *> KXianVector);
vector<FXing *> Clean_All_FX(vector<FXing *> FXVector);
vector<FXing *> Adjust_All_FX(vector<FXing *> FXVector_Clean, vector<KXian*> allKXianVector);
vector<FXing*> Find_Ding_FX_BH(std::vector<KXian*> KXianVector);
vector<FXing*> Find_Di_FX_BH(std::vector<KXian*> KXianVector);
int Find_First_FX_Index_FromALL(vector<FXing*> FXVector);
vector<FXing*> Find_Final_FXVector(vector<FXing*> FXVector);
FXSearchResult Finx_Next_FX_Index_FromAll(int firstFX_Index, vector<FXing*> FXVector);
FXSearchResult FindNextFromFloat(int secondFX_Index, vector<FXing*> FXVector);
vector<FXing*> Find_ALL_FX_FromAll(int first_index, vector<FXing*> FXVector);