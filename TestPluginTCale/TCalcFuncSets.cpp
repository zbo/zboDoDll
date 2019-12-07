#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "Common.h"
#include "SearchApp.h"
#include "FindApp.h"
#include "BI.h"
#include "FX.h"
#include "DUAN.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	//找出分型
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen, pfOUT, pfINa, pfINb, pfINc);
	OutputDebugInfo(KXianVector);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	vector<int> Final_FXVector_Index = Generate_Final_Index(result, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(Final_FXVector_Index, FXVector);
	
	//处理笔产生段
	vector<BI*> BIVector = GenerateBIVector(Final_FXVector);
	vector<BI*> TZXL_Shang = GenerateTZXL_Shang(BIVector);
	vector<TZXLFXing*> TZXL_Shang_Vector = Generate_TZXL_Shang_FX_Vector(TZXL_Shang);
	vector<TZXLFXing*> TZXL_BH_Shang_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Shang, TZXLFXing::DI);
	vector<BI*> TZXL_Xia = GenerateTZXL_Xia(BIVector);
	vector<TZXLFXing*> TZXL_Xia_Vector = Generate_TZXL_Xia_FX_Vector(TZXL_Xia);
	vector<TZXLFXing*> TZXL_BH_Xia_Vector = Generate_TZXL_BH_FX_Vector(TZXL_Xia, TZXLFXing::DING);

	vector<TZXLFXing*> Vector1 = Ordered_TZXL_FX(TZXL_Shang_Vector, TZXL_BH_Shang_Vector);
	vector<TZXLFXing*> Vector2 = Ordered_TZXL_FX(TZXL_Xia_Vector, TZXL_BH_Xia_Vector);
	vector<TZXLFXing*> Ordered_TZXL_Vector = Ordered_TZXL_FX(Vector1, Vector2);
	Ordered_TZXL_Vector = Add_First_TZXL_FX(Ordered_TZXL_Vector, BIVector);
	ofstream out_file("out.txt", ios::trunc);
	out_file << "Ordered_TZXL_FX:" << Ordered_TZXL_Vector.size()<< '\n';
	out_file.close();
	//填充显示数据
	FillinPOutDefault(pfOUT, DataLen);
	FillinPOutDUAN(pfOUT, Ordered_TZXL_Vector);
}


void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen,pfOUT,pfINa,pfINb,pfINc);
	//OutputDebugInfo(KXianVector);
	//归类法找出所有可能的分型
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);

	FXSearchResult result= Find_First_FX_FromALL(FXVector);
	vector<int> Final_FXVector_Index = Generate_Final_Index(result, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(Final_FXVector_Index, FXVector);
	//vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.FirstFX_Index, FXVector);
	FillinPOutDefault(pfOUT,DataLen);
	FillinPOutBI(pfOUT, Final_FXVector);
}


//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] = 
{
	{1,(pPluginFUNC)&TestPlugin1},
	{2,(pPluginFUNC)&TestPlugin2},
	{0,NULL},
};

//导出给TCalc的注册函数
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if(*pFun==NULL)
	{
		(*pFun)=g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}

