#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "Common.h"
#include "SearchApp.h"
#include "FindApp.h"
#include "BI.h"
#include "FX.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	//�ҳ���
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen, pfOUT, pfINa, pfINb, pfINc);
	OutputDebugInfo(KXianVector);
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);
	FXSearchResult result = Find_First_FX_FromALL(FXVector);
	vector<int> Final_FXVector_Index = Generate_Final_Index(result, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(Final_FXVector_Index, FXVector);
	//�����

	FillinPOutDefault(pfOUT, DataLen);
	FillinPOut(pfOUT, FXVector);
}


void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen,pfOUT,pfINa,pfINb,pfINc);
	OutputDebugInfo(KXianVector);
	//���෨�ҳ����п��ܵķ���
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXVector = AdjustGapInFX(FXVector);

	FXSearchResult result= Find_First_FX_FromALL(FXVector);
	vector<int> Final_FXVector_Index = Generate_Final_Index(result, FXVector);
	vector<FXing*> Final_FXVector = Generate_Final_FX(Final_FXVector_Index, FXVector);
	//vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.FirstFX_Index, FXVector);
	FillinPOutDefault(pfOUT,DataLen);
	FillinPOut(pfOUT, Final_FXVector);
}


//���صĺ���
PluginTCalcFuncInfo g_CalcFuncSets[] = 
{
	{1,(pPluginFUNC)&TestPlugin1},
	{2,(pPluginFUNC)&TestPlugin2},
	{0,NULL},
};

//������TCalc��ע�ắ��
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if(*pFun==NULL)
	{
		(*pFun)=g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}

