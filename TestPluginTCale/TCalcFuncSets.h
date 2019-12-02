#ifndef __TCALC_FUNC_SETS
#define __TCALC_FUNC_SETS
#include "PluginTCalcFunc.h"
#include "Common.h"
#include <vector>





#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
	__declspec(dllexport) BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__TCALC_FUNC_SETS

__declspec(dllexport) void TestPlugin2(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc);
