#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "StdAfx.h"

using namespace std;

vector<string> mySplit(const string& str, string sp_string);
BaoHanRela BaoHan(KXian* firstK, KXian* secondK);
void OutputDebugInfo(std::vector<KXian *> KXianVector);
void FillinPOutBI(float* pfOUT, vector<FXing *> FXVector);
void FillinPOutDUAN(float* pfOUT, vector<TZXLFXing*> Ordered_TZXL_Vector);
void FillinPOutDUAN2(float* pfOUT, vector<SearchDuan*> SearchDVector);
void FillinPOutDefault(float* pfOUT, int DataLen);
void PrintFxVector(vector<FXing *> FXVector_Clean);

vector<KXian*> GenerateKXianVector(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc);
vector<KXian*> FindMaxMinFromKXianVector(vector<KXian*> KXianVector);


