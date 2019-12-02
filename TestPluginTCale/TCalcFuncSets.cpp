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
	std::vector<KXian*> KXianVector = GenerateKXianVector(DataLen,pfOUT,pfINa,pfINb,pfINc);
	//找到K线最高点和最低点
	KXian* KX_Max = KXianVector[0];
	KXian* KX_Min = KXianVector[0];
	for(int i=0; i<KXianVector.size(); i++){
		KXian* KX = KXianVector[i];
		if(KXianVector[i]->High>KX_Max->High){
			KX_Max=KXianVector[i];
		}
		if(KXianVector[i]->Low<KX_Min->Low){
			KX_Min=KXianVector[i];
		}
	}
	outfile << "最高点：" << KX_Max->i << "最低点：" << KX_Min->i << "数据总数：" << DataLen <<'\n';
	//找到Max和Min比较靠前的一个，开始有方向的搜索
	//Search_From_Max_Min(KX_Max, KX_Min, KXianVector);
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

