#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "Common.h"
#include "SearchApp.h"
#include "FindApp.h"
#include "BI.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen, pfOUT, pfINa, pfINb, pfINc);
	//归类法找出所有可能的分型
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FillinPOutDefault(pfOUT, DataLen);
	FillinPOut(pfOUT, FXVector);
}


void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	ofstream outfile("out.txt",ios::app);	
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen,pfOUT,pfINa,pfINb,pfINc);
	OutputDebugInfo(KXianVector);
	//归类法找出所有可能的分型
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	FXSearchResult result= Find_First_FX_Index_FromALL(FXVector);
	vector<FXing*> Final_FXVector = Find_ALL_FX_FromAll(result.FirstFX_Index, FXVector);
	FillinPOutDefault(pfOUT,DataLen);
	FillinPOut(pfOUT, Final_FXVector);
	outfile<<"------------------------------------------------"<<'\n';
	outfile.close();
	
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

