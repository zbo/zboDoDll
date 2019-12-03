#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "Common.h"
#include "SearchApp.h"
#include "FindApp.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	for(int i=0;i<DataLen;i++)
		pfOUT[i]=i;
}


void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	ofstream outfile("out.txt",ios::app);	
	vector<KXian*> KXianVector = GenerateKXianVector(DataLen,pfOUT,pfINa,pfINb,pfINc);
	//归类法找出所有可能的分型
	vector<FXing*> FXVector = Find_All_FX(KXianVector);
	vector<FXing*> FXVector_Clean = Clean_All_FX(FXVector);
	vector<FXing*> FXVector_Adjust = Adjust_All_FX(FXVector_Clean, KXianVector);
	
	PrintFxVector(FXVector_Clean);
	FillinPOutDefault(pfOUT,DataLen);
	FillinPOut(pfOUT,FXVector_Clean);
	outfile<<"------------------------------------------------"<<'\n';
	outfile.close();
	OutputDebugInfo(KXianVector);
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

