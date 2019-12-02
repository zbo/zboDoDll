//#include "Common.h"
#include "stdafx.h"
#include <vector>
//#include "Common.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "StdAfx.h"

using namespace std;

__declspec(dllexport) vector<string> mySplit(const string& str, string sp_string)  // split(), str 是要分割的string
{ 
    vector<string> vecString; 
    int sp_stringLen = sp_string.size(); 
    int lastPosition = 0; 
    int index = -1; 
    while(-1 != (index=str.find(sp_string , lastPosition))) 
    { 
        vecString.push_back(str.substr(lastPosition, index - lastPosition)); 
        lastPosition = index +sp_stringLen; 
    } 
    string lastStr = str.substr(lastPosition); 
    if ( !lastStr.empty() ) 
    { 
        vecString.push_back(lastStr); 
    } 
    return vecString; 
} 

BaoHanRela BaoHan(KXian* firstK, KXian* secondK){
	BaoHanRela rela;
	if(firstK->High>=secondK->High&&firstK->Low<=secondK->Low){
		rela.isBaoHan=true;
		rela.BaoHanType = -1;
		return rela;
	}else if(firstK->High<=secondK->High&&firstK->Low>=secondK->Low){
		rela.isBaoHan=true;
		rela.BaoHanType = 1;
		return rela;
	}
	else{
		rela.isBaoHan=false;
		rela.BaoHanType = 0;
		return rela;}
}

void OutputDebugInfo(std::vector<KXian *> &KXianVector)
{
	ofstream debug_file("debug.txt",ios::trunc);
	for(int i=0; i<KXianVector.size(); i++){
		debug_file<<KXianVector[i]->High<<" "<<KXianVector[i]->Low<<'\n';
	}
	debug_file.close();
}

void FillinPOut(float* pfOUT, vector<FXing *> FXVector)
{
	for(int i=0;i<FXVector.size();i++){
		int index = FXVector[i]->Second->i;
		int fxtype = FXVector[i]->FxType==FXing::Ding?1:-1;
		pfOUT[index]=fxtype;
	}
}
__declspec(dllexport) void FillinPOutDefault(float* pfOUT, int DataLen)
{
	for(int i=0;i<DataLen;i++){
		pfOUT[i]=0;
	}
}

void PrintFxVector(vector<FXing *> FXVector_Clean)
{
	ofstream outfile("out.txt",ios::app);
	for(int i=0;i<FXVector_Clean.size();i++){
		outfile<<"==========="<<FXVector_Clean[i]->FxType<<'\n';
		outfile<<"Index: "<<FXVector_Clean[i]->Second->i<<" left: "<< FXVector_Clean[i]->First->i<<" right: "<<FXVector_Clean[i]->Third->i<<'\n';
	}
}

__declspec(dllexport) vector<KXian*> GenerateKXianVector(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc) {
	vector<KXian*> KXianVector;
	//构造K线序列
	for (int i = 0; i < DataLen; i++) {
		KXian* KX = new KXian;
		KX->High = pfINa[i];
		KX->Low = pfINb[i];
		KX->i = i;
		KX->BHan = false;
		KXianVector.push_back(KX);
	}
	return KXianVector;
}