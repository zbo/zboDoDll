#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

BI* CreateBI(FXing* FX1, FXing* FX2);
vector<BI*> GenerateBIVector(vector<FXing*> Final_FXVector);
vector<BI*> GenerateTZXL_Shang(vector<BI*> BIVector);
vector<BI*> GenerateTZXL_Xia(vector<BI*> BIVector);
bool Valid_TZDIFx(BI* Bi_First, BI* Bi_Second, BI* Bi_Third);
bool Valid_TZDINGFx(BI* Bi_First, BI* Bi_Second, BI* Bi_Third);
TZXLFXing* Find_TZXL_DI_FX(vector<BI*> TZXL_Shang, int from);
TZXLFXing* Find_TZXL_DING_FX(vector<BI*> TZXL_Xia, int from);
vector<TZXLFXing*> Ordered_TZXL_FX(vector<TZXLFXing*> TZXL_Shang_Vector, vector<TZXLFXing*> TZXL_Xia_Vector);
vector<TZXLFXing*> Generate_TZXL_Shang_FX_Vector(vector<BI*> TZXL_Shang);
vector<TZXLFXing*> Generate_TZXL_Xia_FX_Vector(vector<BI*> TZXL_Xia);
vector<TZXLFXing*> Generate_TZXL_BH_FX_Vector(vector<BI*> BIVector, TZXLFXing::TZXLFXingType Needs);
vector<BI*> ProcessBaoHanTZXL(vector<BI*> BIVector_Clean, BI* BiIn, BaoHanRela baoHanRela, TZXLFXing::TZXLFXingType Needs);
BaoHanRela BaoHanTZXL(BI* firstBI, BI* secondBI);
vector<TZXLFXing*> Add_First_TZXL_FX(vector<TZXLFXing*> Ordered_TZXL_Vector, vector<BI*> BIVector);




