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
